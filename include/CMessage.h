#ifndef CMESSAGE_H
#define CMESSAGE_H

#include <map>

#define CMessageMgrInst CMessageMgr::getInstance()

class CMessageMgr {
 public:
  static CMessageMgr *getInstance();

  std::string getIdFilename (const std::string &id);
  std::string getNumFilename(const std::string &id);

 private:
  CMessageMgr();
 ~CMessageMgr();

 private:
  typedef std::map<std::string,int> IdMap;

  IdMap idMap_;
};

class CMessage {
 public:
  static bool isActive(const std::string &id);

  CMessage(const std::string &id);
 ~CMessage();

  bool sendClientMessage(const std::string &msg);
  bool sendClientData(int type, const char *data, int len);

  bool recvClientMessage(std::string &msg);
  bool recvClientData(int &type, char* &data, int &len);

  bool sendServerMessage(const std::string &msg, int errorCode=0);
  bool sendServerData(int type, const char *data, int len, int errorCode=0);

  bool recvServerMessage(std::string &msg, int *errorCode);
  bool recvServerData(int &type, char* &data, int &len);

  bool sendClientMessageAndRecv(const std::string &msg, std::string &reply);

 private:
  int createSharedMem();

  int  getShmId();
  void setShmId(int integer);

  void setShmNum(int integer);
  void incShmNum();
  bool decShmNum();

  static int getShmId(const std::string &idFilename);

 private:
  enum class Type {
    STRING
  };

  static const int MAX_DATA = 32768;

  struct ClientMessageData {
    int  pending;
    int  type;
    char data[MAX_DATA];
    int  len;
  };

  struct ServerMessageData {
    int  pending;
    int  type;
    char data[MAX_DATA];
    int  len;
    int  errorCode;
  };

  struct MessageData {
    ClientMessageData clientData;
    ServerMessageData serverData;
  };

  std::string id_;
  int         shmId_ { 0 };
  std::string idFilename_;
  std::string numFilename_;
  bool        debug_ { false };
};

#endif
