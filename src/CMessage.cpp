#include <CMessage.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <COSTimer.h>

#include <iostream>
#include <cstring>
#include <cassert>
#include <sys/ipc.h>
#include <sys/shm.h>

class CMessageLock {
 public:
  CMessageLock(int id);
 ~CMessageLock();

  void *getData() const { return data_; }

 private:
  static bool locked_;

  void* data_ { nullptr };
};

//---------

CMessageMgr *
CMessageMgr::
getInstance()
{
  static CMessageMgr *instance;

  if (! instance)
    instance = new CMessageMgr;

  return instance;
}

CMessageMgr::
CMessageMgr()
{
}

CMessageMgr::
~CMessageMgr()
{
}

std::string
CMessageMgr::
getIdFilename(const std::string &id)
{
  std::string id1 = CStrUtil::toUpper(CStrUtil::stripSpaces(id));

  return "/tmp/" + id1 + "_SHM_ID";
}

std::string
CMessageMgr::
getNumFilename(const std::string &id)
{
  std::string id1 = CStrUtil::toUpper(CStrUtil::stripSpaces(id));

  return "/tmp/" + id1 + "_SHM_NUM";
}

//---------

bool
CMessage::
isActive(const std::string &id)
{
  std::string idFilename = CMessageMgrInst->getIdFilename(id);

  int shmId = getShmId(idFilename);

  return (shmId != 0);
}

CMessage::
CMessage(const std::string &id) :
 id_(id), debug_(false)
{
  if (getenv("CMESSAGE_DEBUG"))
    debug_ = true;

  idFilename_  = CMessageMgrInst->getIdFilename (id_);
  numFilename_ = CMessageMgrInst->getNumFilename(id_);

  shmId_ = getShmId(idFilename_);

  if (shmId_ == 0) {
    shmId_ = createSharedMem();

    setShmId (shmId_);
    setShmNum(1);
  }
  else
    incShmNum();
}

CMessage::
~CMessage()
{
  if (decShmNum()) {
    if (debug_)
      std::cerr << "remove shm id " << shmId_ << std::endl;

    shmctl(shmId_, IPC_RMID, nullptr);

    if (debug_)
      std::cerr << "remove files " << idFilename_ << " " << numFilename_ << std::endl;

    CFile::remove(idFilename_);
    CFile::remove(numFilename_);
  }
}

int
CMessage::
createSharedMem()
{
  int shmId = shmget(IPC_PRIVATE, sizeof(MessageData), IPC_CREAT | IPC_EXCL | 0600);

  if (shmId == -1)
    perror("shmget");

  return shmId;
}

bool
CMessage::
sendClientMessage(const std::string &msg)
{
  return sendClientData((int) Type::STRING, msg.c_str(), msg.size() + 1);
}

bool
CMessage::
sendClientData(int type, const char *data, int len)
{
  CMessageLock lock(shmId_);

  MessageData *messageData = (MessageData *) lock.getData();
  if (! messageData) return false;

  ClientMessageData *clientData = &messageData->clientData;
  if (clientData->pending) return false;

  clientData->type = type;
  clientData->len  = len;

  if (clientData->len >= (int) sizeof(clientData->data))
    return false;

  memcpy(clientData->data, data, clientData->len);

  clientData->pending = true;

  return true;
}

bool
CMessage::
recvClientMessage(std::string &msg)
{
  CMessageLock lock(shmId_);

  MessageData *messageData = (MessageData *) lock.getData();
  if (! messageData) return false;

  ClientMessageData *clientData = &messageData->clientData;
  if (! clientData->pending) return false;

  if (clientData->type == (int) Type::STRING)
    msg = std::string(clientData->data, clientData->len);

  clientData->pending = false;

  return true;
}

bool
CMessage::
recvClientData(int &type, char* &data, int &len)
{
  CMessageLock lock(shmId_);

  MessageData *messageData = (MessageData *) lock.getData();
  if (! messageData) return false;

  ClientMessageData *clientData = &messageData->clientData;
  if (! clientData->pending) return false;

  type = clientData->type;
  len  = clientData->len;
  data = new char [len];

  memcpy(data, clientData->data, len);

  return true;
}

bool
CMessage::
sendServerMessage(const std::string &msg, int errorCode)
{
  return sendServerData((int) Type::STRING, msg.c_str(), msg.size() + 1, errorCode);
}

bool
CMessage::
sendServerData(int type, const char *data, int len, int errorCode)
{
  CMessageLock lock(shmId_);

  MessageData *messageData = (MessageData *) lock.getData();
  if (! messageData) return false;

  ServerMessageData *serverData = &messageData->serverData;
  if (serverData->pending) return false;

  serverData->type = type;
  serverData->len  = len;

  if (serverData->len >= (int) sizeof(serverData->data))
    return false;

  memcpy(serverData->data, data, serverData->len);

  serverData->errorCode = errorCode;
  serverData->pending   = true;

  return true;
}

bool
CMessage::
recvServerMessage(std::string &msg, int *errorCode)
{
  CMessageLock lock(shmId_);

  MessageData *messageData = (MessageData *) lock.getData();
  if (! messageData) return false;

  ServerMessageData *serverData = &messageData->serverData;
  if (! serverData->pending) return false;

  if (serverData->type == (int) Type::STRING)
    msg = std::string(serverData->data, serverData->len);

  *errorCode = serverData->errorCode;

  serverData->pending = false;

  return true;
}

bool
CMessage::
recvServerData(int &type, char* &data, int &len)
{
  CMessageLock lock(shmId_);

  MessageData *messageData = (MessageData *) lock.getData();
  if (! messageData) return false;

  ServerMessageData *serverData = &messageData->serverData;
  if (! serverData->pending) return false;

  type = serverData->type;
  len  = serverData->len;
  data = new char [len];

  memcpy(data, serverData->data, len);

  return true;
}

bool
CMessage::
sendClientMessageAndRecv(const std::string &msg, std::string &reply)
{
  reply = "";

  sendClientMessage(msg);

  for (int i = 0; i < 10000; ++i) {
    COSTimer::msleep(50);

    int errorCode;

    if (recvServerMessage(reply, &errorCode))
      return true;
  }

  return false;
}

int
CMessage::
getShmId()
{
  return getShmId(idFilename_);
}

int
CMessage::
getShmId(const std::string &idFilename)
{
  int integer = 0;

  if (CFile::exists(idFilename)) {
    CFile file(idFilename);

    std::string line;

    file.readLine(line);

    CStrUtil::toInteger(line, &integer);
  }

  CMessageLock lock(integer);

  MessageData *messageData = (MessageData *) lock.getData();
  if (! messageData) return 0;

  return integer;
}

void
CMessage::
setShmId(int integer)
{
  CFile file(idFilename_);

  std::string line = CStrUtil::toString(integer) + "\n";

  file.write(line);

  if (debug_)
    std::cerr << "setShmId " << integer << std::endl;
}

void
CMessage::
setShmNum(int integer)
{
  CMessageLock lock(shmId_);

  CFile file(numFilename_);

  std::string line = CStrUtil::toString(integer) + "\n";

  file.write(line);

  if (debug_)
    std::cerr << "setShmNum " << integer << std::endl;
}

void
CMessage::
incShmNum()
{
  CMessageLock lock(shmId_);

  int integer = 0;

  if (CFile::exists(numFilename_)) {
    CFile file(numFilename_);

    std::string line;

    file.readLine(line);

    CStrUtil::toInteger(line, &integer);

    ++integer;

    file.rewind();

    line = CStrUtil::toString(integer) + "\n";

    file.write(line);
  }
  else {
    integer = 1;

    CFile file(numFilename_);

    std::string line = CStrUtil::toString(integer) + "\n";

    file.write(line);
  }

  if (debug_)
    std::cerr << "incShmNum " << integer << std::endl;
}

bool
CMessage::
decShmNum()
{
  CMessageLock lock(shmId_);

  int integer = 0;

  if (CFile::exists(numFilename_)) {
    CFile file(numFilename_);

    std::string line;

    file.readLine(line);

    CStrUtil::toInteger(line, &integer);

    --integer;

    file.rewind();

    line = CStrUtil::toString(integer) + "\n";

    file.write(line);
  }
  else {
    integer = 0;

    CFile file(numFilename_);

    std::string line = CStrUtil::toString(integer) + "\n";

    file.write(line);
  }

  if (debug_)
    std::cerr << "decShmNum " << integer << std::endl;

  return (integer == 0);
}

//--------

bool CMessageLock::locked_ = false;

CMessageLock::
CMessageLock(int id)
{
  assert(! locked_);

  locked_ = true;

  data_ = shmat(id, nullptr, SHM_RND);

  if (data_ == (void *) -1)
    data_ = nullptr;
}

CMessageLock::
~CMessageLock()
{
  assert(locked_);

  locked_ = false;

  shmdt(data_);
}
