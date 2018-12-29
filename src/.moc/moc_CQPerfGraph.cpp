/****************************************************************************
** Meta object code from reading C++ file 'CQPerfGraph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/CQPerfGraph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQPerfGraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CQPerfDialog_t {
    QByteArrayData data[24];
    char stringdata0[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQPerfDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQPerfDialog_t qt_meta_stringdata_CQPerfDialog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CQPerfDialog"
QT_MOC_LITERAL(1, 13, 10), // "setTimeout"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 1), // "t"
QT_MOC_LITERAL(4, 27, 7), // "setName"
QT_MOC_LITERAL(5, 35, 4), // "name"
QT_MOC_LITERAL(6, 40, 8), // "setNames"
QT_MOC_LITERAL(7, 49, 5), // "names"
QT_MOC_LITERAL(8, 55, 13), // "updateWidgets"
QT_MOC_LITERAL(9, 69, 11), // "enabledSlot"
QT_MOC_LITERAL(10, 81, 5), // "state"
QT_MOC_LITERAL(11, 87, 9), // "debugSlot"
QT_MOC_LITERAL(12, 97, 13), // "typeComboSlot"
QT_MOC_LITERAL(13, 111, 3), // "ind"
QT_MOC_LITERAL(14, 115, 14), // "shapeComboSlot"
QT_MOC_LITERAL(15, 130, 14), // "valueComboSlot"
QT_MOC_LITERAL(16, 145, 14), // "windowSizeSlot"
QT_MOC_LITERAL(17, 160, 4), // "size"
QT_MOC_LITERAL(18, 165, 10), // "recordSlot"
QT_MOC_LITERAL(19, 176, 9), // "stateSlot"
QT_MOC_LITERAL(20, 186, 11), // "zoomOutSlot"
QT_MOC_LITERAL(21, 198, 10), // "zoomInSlot"
QT_MOC_LITERAL(22, 209, 10), // "scrollSlot"
QT_MOC_LITERAL(23, 220, 7) // "timeout"

    },
    "CQPerfDialog\0setTimeout\0\0t\0setName\0"
    "name\0setNames\0names\0updateWidgets\0"
    "enabledSlot\0state\0debugSlot\0typeComboSlot\0"
    "ind\0shapeComboSlot\0valueComboSlot\0"
    "windowSizeSlot\0size\0recordSlot\0stateSlot\0"
    "zoomOutSlot\0zoomInSlot\0scrollSlot\0"
    "timeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQPerfDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       1,  124, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x0a /* Public */,
       4,    1,   92,    2, 0x08 /* Private */,
       6,    1,   95,    2, 0x08 /* Private */,
       8,    0,   98,    2, 0x08 /* Private */,
       9,    1,   99,    2, 0x08 /* Private */,
      11,    1,  102,    2, 0x08 /* Private */,
      12,    1,  105,    2, 0x08 /* Private */,
      14,    1,  108,    2, 0x08 /* Private */,
      15,    1,  111,    2, 0x08 /* Private */,
      16,    1,  114,    2, 0x08 /* Private */,
      18,    0,  117,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    0,  119,    2, 0x08 /* Private */,
      21,    0,  120,    2, 0x08 /* Private */,
      22,    1,  121,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QStringList,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // properties: name, type, flags
      23, QMetaType::Int, 0x00095103,

       0        // eod
};

void CQPerfDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQPerfDialog *_t = static_cast<CQPerfDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setTimeout((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->setNames((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: _t->updateWidgets(); break;
        case 4: _t->enabledSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->debugSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->typeComboSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->shapeComboSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->valueComboSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->windowSizeSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->recordSlot(); break;
        case 11: _t->stateSlot(); break;
        case 12: _t->zoomOutSlot(); break;
        case 13: _t->zoomInSlot(); break;
        case 14: _t->scrollSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        CQPerfDialog *_t = static_cast<CQPerfDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->timeout(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        CQPerfDialog *_t = static_cast<CQPerfDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTimeout(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject CQPerfDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CQPerfDialog.data,
      qt_meta_data_CQPerfDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CQPerfDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQPerfDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CQPerfDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CQPerfDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_CQPerfGraph_t {
    QByteArrayData data[22];
    char stringdata0[246];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQPerfGraph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQPerfGraph_t qt_meta_stringdata_CQPerfGraph = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CQPerfGraph"
QT_MOC_LITERAL(1, 12, 13), // "setWindowSize"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 1), // "i"
QT_MOC_LITERAL(4, 29, 15), // "setNumIntervals"
QT_MOC_LITERAL(5, 45, 12), // "setShowTotal"
QT_MOC_LITERAL(6, 58, 1), // "b"
QT_MOC_LITERAL(7, 60, 12), // "setShowDepth"
QT_MOC_LITERAL(8, 73, 16), // "setShowRecording"
QT_MOC_LITERAL(9, 90, 13), // "setShowPoints"
QT_MOC_LITERAL(10, 104, 12), // "setShowRects"
QT_MOC_LITERAL(11, 117, 14), // "setShowElapsed"
QT_MOC_LITERAL(12, 132, 12), // "setShowCount"
QT_MOC_LITERAL(13, 145, 9), // "showTotal"
QT_MOC_LITERAL(14, 155, 9), // "showDepth"
QT_MOC_LITERAL(15, 165, 13), // "showRecording"
QT_MOC_LITERAL(16, 179, 10), // "showPoints"
QT_MOC_LITERAL(17, 190, 9), // "showRects"
QT_MOC_LITERAL(18, 200, 11), // "showElapsed"
QT_MOC_LITERAL(19, 212, 9), // "showCount"
QT_MOC_LITERAL(20, 222, 10), // "windowSize"
QT_MOC_LITERAL(21, 233, 12) // "numIntervals"

    },
    "CQPerfGraph\0setWindowSize\0\0i\0"
    "setNumIntervals\0setShowTotal\0b\0"
    "setShowDepth\0setShowRecording\0"
    "setShowPoints\0setShowRects\0setShowElapsed\0"
    "setShowCount\0showTotal\0showDepth\0"
    "showRecording\0showPoints\0showRects\0"
    "showElapsed\0showCount\0windowSize\0"
    "numIntervals"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQPerfGraph[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       9,   86, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       4,    1,   62,    2, 0x0a /* Public */,
       5,    1,   65,    2, 0x0a /* Public */,
       7,    1,   68,    2, 0x0a /* Public */,
       8,    1,   71,    2, 0x0a /* Public */,
       9,    1,   74,    2, 0x0a /* Public */,
      10,    1,   77,    2, 0x0a /* Public */,
      11,    1,   80,    2, 0x0a /* Public */,
      12,    1,   83,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,

 // properties: name, type, flags
      13, QMetaType::Bool, 0x00095103,
      14, QMetaType::Bool, 0x00095103,
      15, QMetaType::Bool, 0x00095103,
      16, QMetaType::Bool, 0x00095103,
      17, QMetaType::Bool, 0x00095103,
      18, QMetaType::Bool, 0x00095003,
      19, QMetaType::Bool, 0x00095103,
      20, QMetaType::Int, 0x00095103,
      21, QMetaType::Int, 0x00095103,

       0        // eod
};

void CQPerfGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQPerfGraph *_t = static_cast<CQPerfGraph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setWindowSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setNumIntervals((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setShowTotal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setShowDepth((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setShowRecording((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setShowPoints((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setShowRects((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setShowElapsed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setShowCount((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        CQPerfGraph *_t = static_cast<CQPerfGraph *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isShowTotal(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isShowDepth(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isShowRecording(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isShowPoints(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->isShowRects(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->isShowElapsed(); break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->isShowCount(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->windowSize(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->numIntervals(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        CQPerfGraph *_t = static_cast<CQPerfGraph *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setShowTotal(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setShowDepth(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setShowRecording(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setShowPoints(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setShowRects(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setShowRects(*reinterpret_cast< bool*>(_v)); break;
        case 6: _t->setShowCount(*reinterpret_cast< bool*>(_v)); break;
        case 7: _t->setWindowSize(*reinterpret_cast< int*>(_v)); break;
        case 8: _t->setNumIntervals(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject CQPerfGraph::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_CQPerfGraph.data,
      qt_meta_data_CQPerfGraph,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CQPerfGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQPerfGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CQPerfGraph.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int CQPerfGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_CQPerfList_t {
    QByteArrayData data[13];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQPerfList_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQPerfList_t qt_meta_stringdata_CQPerfList = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CQPerfList"
QT_MOC_LITERAL(1, 11, 12), // "nameSelected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "name"
QT_MOC_LITERAL(4, 30, 13), // "namesSelected"
QT_MOC_LITERAL(5, 44, 5), // "names"
QT_MOC_LITERAL(6, 50, 6), // "reload"
QT_MOC_LITERAL(7, 57, 7), // "refresh"
QT_MOC_LITERAL(8, 65, 19), // "selectionChangeSlot"
QT_MOC_LITERAL(9, 85, 9), // "clickSlot"
QT_MOC_LITERAL(10, 95, 3), // "row"
QT_MOC_LITERAL(11, 99, 6), // "column"
QT_MOC_LITERAL(12, 106, 12) // "singleSelect"

    },
    "CQPerfList\0nameSelected\0\0name\0"
    "namesSelected\0names\0reload\0refresh\0"
    "selectionChangeSlot\0clickSlot\0row\0"
    "column\0singleSelect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQPerfList[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       1,   58, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   50,    2, 0x0a /* Public */,
       7,    0,   51,    2, 0x0a /* Public */,
       8,    0,   52,    2, 0x08 /* Private */,
       9,    2,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QStringList,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,

 // properties: name, type, flags
      12, QMetaType::Bool, 0x00095103,

       0        // eod
};

void CQPerfList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQPerfList *_t = static_cast<CQPerfList *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->nameSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->namesSelected((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->reload(); break;
        case 3: _t->refresh(); break;
        case 4: _t->selectionChangeSlot(); break;
        case 5: _t->clickSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (CQPerfList::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CQPerfList::nameSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CQPerfList::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CQPerfList::namesSelected)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        CQPerfList *_t = static_cast<CQPerfList *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isSingleSelect(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        CQPerfList *_t = static_cast<CQPerfList *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSingleSelect(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject CQPerfList::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_CQPerfList.data,
      qt_meta_data_CQPerfList,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CQPerfList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQPerfList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CQPerfList.stringdata0))
        return static_cast<void*>(this);
    return QTableWidget::qt_metacast(_clname);
}

int CQPerfList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CQPerfList::nameSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CQPerfList::namesSelected(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
