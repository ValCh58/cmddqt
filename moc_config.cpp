/****************************************************************************
** Meta object code from reading C++ file 'config.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "config.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'config.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_config_t {
    QByteArrayData data[11];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_config_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_config_t qt_meta_stringdata_config = {
    {
QT_MOC_LITERAL(0, 0, 6), // "config"
QT_MOC_LITERAL(1, 7, 11), // "changeLine1"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 3), // "idx"
QT_MOC_LITERAL(4, 24, 11), // "changeLine2"
QT_MOC_LITERAL(5, 36, 9), // "changeBod"
QT_MOC_LITERAL(6, 46, 14), // "changeByteSize"
QT_MOC_LITERAL(7, 61, 3), // "val"
QT_MOC_LITERAL(8, 65, 12), // "changeParity"
QT_MOC_LITERAL(9, 78, 13), // "changeStopBit"
QT_MOC_LITERAL(10, 92, 10) // "saveConfig"

    },
    "config\0changeLine1\0\0idx\0changeLine2\0"
    "changeBod\0changeByteSize\0val\0changeParity\0"
    "changeStopBit\0saveConfig"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_config[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x08 /* Private */,
       4,    1,   52,    2, 0x08 /* Private */,
       5,    1,   55,    2, 0x08 /* Private */,
       6,    1,   58,    2, 0x08 /* Private */,
       8,    1,   61,    2, 0x08 /* Private */,
       9,    1,   64,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,

       0        // eod
};

void config::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<config *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->changeLine1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->changeLine2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeBod((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->changeByteSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->changeParity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->changeStopBit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->saveConfig(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject config::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_config.data,
    qt_meta_data_config,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *config::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *config::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_config.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int config::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
