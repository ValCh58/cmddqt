/****************************************************************************
** Meta object code from reading C++ file 'comdtords.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "comdtords.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'comdtords.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CmdToRds_t {
    QByteArrayData data[17];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CmdToRds_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CmdToRds_t qt_meta_stringdata_CmdToRds = {
    {
QT_MOC_LITERAL(0, 0, 8), // "CmdToRds"
QT_MOC_LITERAL(1, 9, 8), // "sigError"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 1), // "s"
QT_MOC_LITERAL(4, 21, 14), // "sigSetIndColor"
QT_MOC_LITERAL(5, 36, 3), // "wrd"
QT_MOC_LITERAL(6, 40, 9), // "sigReqPrt"
QT_MOC_LITERAL(7, 50, 12), // "DataModules*"
QT_MOC_LITERAL(8, 63, 3), // "mod"
QT_MOC_LITERAL(9, 67, 3), // "cmd"
QT_MOC_LITERAL(10, 71, 11), // "discretRDS*"
QT_MOC_LITERAL(11, 83, 3), // "rds"
QT_MOC_LITERAL(12, 87, 11), // "sigSendWord"
QT_MOC_LITERAL(13, 99, 15), // "startWriteToRDS"
QT_MOC_LITERAL(14, 115, 9), // "testTimer"
QT_MOC_LITERAL(15, 125, 9), // "slReqPort"
QT_MOC_LITERAL(16, 135, 10) // "slSendWord"

    },
    "CmdToRds\0sigError\0\0s\0sigSetIndColor\0"
    "wrd\0sigReqPrt\0DataModules*\0mod\0cmd\0"
    "discretRDS*\0rds\0sigSendWord\0startWriteToRDS\0"
    "testTimer\0slReqPort\0slSendWord"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CmdToRds[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    3,   60,    2, 0x06 /* Public */,
      12,    1,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   70,    2, 0x0a /* Public */,
      14,    0,   71,    2, 0x0a /* Public */,
      15,    3,   72,    2, 0x0a /* Public */,
      16,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::UInt,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int, 0x80000000 | 10,    8,    9,   11,
    QMetaType::Void, QMetaType::ULong,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int, 0x80000000 | 10,    8,    9,   11,
    QMetaType::Void, QMetaType::ULong,    5,

       0        // eod
};

void CmdToRds::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CmdToRds *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sigError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sigSetIndColor((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 2: _t->sigReqPrt((*reinterpret_cast< DataModules*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< discretRDS*(*)>(_a[3]))); break;
        case 3: _t->sigSendWord((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 4: _t->startWriteToRDS(); break;
        case 5: _t->testTimer(); break;
        case 6: _t->slReqPort((*reinterpret_cast< DataModules*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< discretRDS*(*)>(_a[3]))); break;
        case 7: _t->slSendWord((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CmdToRds::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CmdToRds::sigError)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CmdToRds::*)(quint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CmdToRds::sigSetIndColor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CmdToRds::*)(DataModules * , int , discretRDS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CmdToRds::sigReqPrt)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CmdToRds::*)(ulong );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CmdToRds::sigSendWord)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CmdToRds::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CmdToRds.data,
    qt_meta_data_CmdToRds,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CmdToRds::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CmdToRds::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CmdToRds.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CmdToRds::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CmdToRds::sigError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CmdToRds::sigSetIndColor(quint32 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CmdToRds::sigReqPrt(DataModules * _t1, int _t2, discretRDS * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CmdToRds::sigSendWord(ulong _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
