/****************************************************************************
** Meta object code from reading C++ file 'calibratedialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "calibratedialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calibratedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalibrateDialog_t {
    QByteArrayData data[21];
    char stringdata0[284];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalibrateDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalibrateDialog_t qt_meta_stringdata_CalibrateDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CalibrateDialog"
QT_MOC_LITERAL(1, 16, 10), // "sigMeasur0"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "DataModules*"
QT_MOC_LITERAL(4, 41, 5), // "bool*"
QT_MOC_LITERAL(5, 47, 5), // "uint*"
QT_MOC_LITERAL(6, 53, 10), // "sigMeasur1"
QT_MOC_LITERAL(7, 64, 14), // "changeProgress"
QT_MOC_LITERAL(8, 79, 18), // "sigWriteCalFactor0"
QT_MOC_LITERAL(9, 98, 18), // "sigWriteCalFactor1"
QT_MOC_LITERAL(10, 117, 17), // "sigResetCalCoeff0"
QT_MOC_LITERAL(11, 135, 17), // "sigResetCalCoeff1"
QT_MOC_LITERAL(12, 153, 17), // "sigControlMeasur0"
QT_MOC_LITERAL(13, 171, 8), // "LogFile*"
QT_MOC_LITERAL(14, 180, 17), // "sigControlMeasur1"
QT_MOC_LITERAL(15, 198, 13), // "makeCalibrate"
QT_MOC_LITERAL(16, 212, 13), // "processMeasur"
QT_MOC_LITERAL(17, 226, 4), // "type"
QT_MOC_LITERAL(18, 231, 22), // "writeCalibrationFactor"
QT_MOC_LITERAL(19, 254, 15), // "resetCalibCoeff"
QT_MOC_LITERAL(20, 270, 13) // "ControlMeasur"

    },
    "CalibrateDialog\0sigMeasur0\0\0DataModules*\0"
    "bool*\0uint*\0sigMeasur1\0changeProgress\0"
    "sigWriteCalFactor0\0sigWriteCalFactor1\0"
    "sigResetCalCoeff0\0sigResetCalCoeff1\0"
    "sigControlMeasur0\0LogFile*\0sigControlMeasur1\0"
    "makeCalibrate\0processMeasur\0type\0"
    "writeCalibrationFactor\0resetCalibCoeff\0"
    "ControlMeasur"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalibrateDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   84,    2, 0x06 /* Public */,
       6,    4,   93,    2, 0x06 /* Public */,
       7,    1,  102,    2, 0x06 /* Public */,
       8,    5,  105,    2, 0x06 /* Public */,
       9,    5,  116,    2, 0x06 /* Public */,
      10,    2,  127,    2, 0x06 /* Public */,
      11,    2,  132,    2, 0x06 /* Public */,
      12,    5,  137,    2, 0x06 /* Public */,
      14,    5,  148,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,  159,    2, 0x08 /* Private */,
      16,    1,  160,    2, 0x08 /* Private */,
      18,    0,  163,    2, 0x08 /* Private */,
      19,    0,  164,    2, 0x08 /* Private */,
      20,    0,  165,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 5,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 5,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 5,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 5,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 5, 0x80000000 | 13,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 5, 0x80000000 | 13,    2,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CalibrateDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrateDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sigMeasur0((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< DataModules*(*)>(_a[2])),(*reinterpret_cast< bool*(*)>(_a[3])),(*reinterpret_cast< uint*(*)>(_a[4]))); break;
        case 1: _t->sigMeasur1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< DataModules*(*)>(_a[2])),(*reinterpret_cast< bool*(*)>(_a[3])),(*reinterpret_cast< uint*(*)>(_a[4]))); break;
        case 2: _t->changeProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sigWriteCalFactor0((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< uint*(*)>(_a[5]))); break;
        case 4: _t->sigWriteCalFactor1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< uint*(*)>(_a[5]))); break;
        case 5: _t->sigResetCalCoeff0((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->sigResetCalCoeff1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->sigControlMeasur0((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< uint*(*)>(_a[4])),(*reinterpret_cast< LogFile*(*)>(_a[5]))); break;
        case 8: _t->sigControlMeasur1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< uint*(*)>(_a[4])),(*reinterpret_cast< LogFile*(*)>(_a[5]))); break;
        case 9: _t->makeCalibrate(); break;
        case 10: _t->processMeasur((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->writeCalibrationFactor(); break;
        case 12: _t->resetCalibCoeff(); break;
        case 13: _t->ControlMeasur(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalibrateDialog::*)(int , DataModules * , bool * , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigMeasur0)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int , DataModules * , bool * , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigMeasur1)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::changeProgress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int , int , int , int , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigWriteCalFactor0)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int , int , int , int , uint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigWriteCalFactor1)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigResetCalCoeff0)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigResetCalCoeff1)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int , int , int , uint * , LogFile * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigControlMeasur0)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (CalibrateDialog::*)(int , int , int , uint * , LogFile * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrateDialog::sigControlMeasur1)) {
                *result = 8;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CalibrateDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CalibrateDialog.data,
    qt_meta_data_CalibrateDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CalibrateDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrateDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalibrateDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CalibrateDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CalibrateDialog::sigMeasur0(int _t1, DataModules * _t2, bool * _t3, uint * _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CalibrateDialog::sigMeasur1(int _t1, DataModules * _t2, bool * _t3, uint * _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CalibrateDialog::changeProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CalibrateDialog::sigWriteCalFactor0(int _t1, int _t2, int _t3, int _t4, uint * _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CalibrateDialog::sigWriteCalFactor1(int _t1, int _t2, int _t3, int _t4, uint * _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CalibrateDialog::sigResetCalCoeff0(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CalibrateDialog::sigResetCalCoeff1(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CalibrateDialog::sigControlMeasur0(int _t1, int _t2, int _t3, uint * _t4, LogFile * _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CalibrateDialog::sigControlMeasur1(int _t1, int _t2, int _t3, uint * _t4, LogFile * _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
