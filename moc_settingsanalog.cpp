/****************************************************************************
** Meta object code from reading C++ file 'settingsanalog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "settingsanalog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingsanalog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SettingsAnalog_t {
    QByteArrayData data[9];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingsAnalog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingsAnalog_t qt_meta_stringdata_SettingsAnalog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SettingsAnalog"
QT_MOC_LITERAL(1, 15, 16), // "sigReadSettings0"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 12), // "DataModules*"
QT_MOC_LITERAL(4, 46, 5), // "uint*"
QT_MOC_LITERAL(5, 52, 16), // "sigReadSettings1"
QT_MOC_LITERAL(6, 69, 17), // "sigWriteSettings0"
QT_MOC_LITERAL(7, 87, 17), // "sigWriteSettings1"
QT_MOC_LITERAL(8, 105, 13) // "writeSettings"

    },
    "SettingsAnalog\0sigReadSettings0\0\0"
    "DataModules*\0uint*\0sigReadSettings1\0"
    "sigWriteSettings0\0sigWriteSettings1\0"
    "writeSettings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingsAnalog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,
       5,    3,   46,    2, 0x06 /* Public */,
       6,    3,   53,    2, 0x06 /* Public */,
       7,    3,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 4,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 4,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 4,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 4,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void SettingsAnalog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingsAnalog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sigReadSettings0((*reinterpret_cast< DataModules*(*)>(_a[1])),(*reinterpret_cast< uint*(*)>(_a[2])),(*reinterpret_cast< uint*(*)>(_a[3]))); break;
        case 1: _t->sigReadSettings1((*reinterpret_cast< DataModules*(*)>(_a[1])),(*reinterpret_cast< uint*(*)>(_a[2])),(*reinterpret_cast< uint*(*)>(_a[3]))); break;
        case 2: _t->sigWriteSettings0((*reinterpret_cast< DataModules*(*)>(_a[1])),(*reinterpret_cast< uint*(*)>(_a[2])),(*reinterpret_cast< uint*(*)>(_a[3]))); break;
        case 3: _t->sigWriteSettings1((*reinterpret_cast< DataModules*(*)>(_a[1])),(*reinterpret_cast< uint*(*)>(_a[2])),(*reinterpret_cast< uint*(*)>(_a[3]))); break;
        case 4: _t->writeSettings(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SettingsAnalog::*)(DataModules * , uint * , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsAnalog::sigReadSettings0)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SettingsAnalog::*)(DataModules * , uint * , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsAnalog::sigReadSettings1)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SettingsAnalog::*)(DataModules * , uint * , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsAnalog::sigWriteSettings0)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SettingsAnalog::*)(DataModules * , uint * , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsAnalog::sigWriteSettings1)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SettingsAnalog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SettingsAnalog.data,
    qt_meta_data_SettingsAnalog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SettingsAnalog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsAnalog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsAnalog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SettingsAnalog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SettingsAnalog::sigReadSettings0(DataModules * _t1, uint * _t2, uint * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SettingsAnalog::sigReadSettings1(DataModules * _t1, uint * _t2, uint * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SettingsAnalog::sigWriteSettings0(DataModules * _t1, uint * _t2, uint * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SettingsAnalog::sigWriteSettings1(DataModules * _t1, uint * _t2, uint * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
