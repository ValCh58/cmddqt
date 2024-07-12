/****************************************************************************
** Meta object code from reading C++ file 'calibrationcheck.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "calibrationcheck.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calibrationcheck.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalibrationCheck_t {
    QByteArrayData data[9];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalibrationCheck_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalibrationCheck_t qt_meta_stringdata_CalibrationCheck = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CalibrationCheck"
QT_MOC_LITERAL(1, 17, 8), // "getClick"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 2), // "id"
QT_MOC_LITERAL(4, 30, 9), // "execCheck"
QT_MOC_LITERAL(5, 40, 17), // "setButtonReadADC1"
QT_MOC_LITERAL(6, 58, 17), // "setButtonReadADC2"
QT_MOC_LITERAL(7, 76, 14), // "getButtonCalc1"
QT_MOC_LITERAL(8, 91, 14) // "getButtonCalc2"

    },
    "CalibrationCheck\0getClick\0\0id\0execCheck\0"
    "setButtonReadADC1\0setButtonReadADC2\0"
    "getButtonCalc1\0getButtonCalc2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalibrationCheck[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       4,    0,   47,    2, 0x08 /* Private */,
       5,    0,   48,    2, 0x08 /* Private */,
       6,    0,   49,    2, 0x08 /* Private */,
       7,    0,   50,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CalibrationCheck::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrationCheck *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->getClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->execCheck(); break;
        case 2: _t->setButtonReadADC1(); break;
        case 3: _t->setButtonReadADC2(); break;
        case 4: _t->getButtonCalc1(); break;
        case 5: _t->getButtonCalc2(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CalibrationCheck::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CalibrationCheck.data,
    qt_meta_data_CalibrationCheck,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CalibrationCheck::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrationCheck::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalibrationCheck.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CalibrationCheck::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
