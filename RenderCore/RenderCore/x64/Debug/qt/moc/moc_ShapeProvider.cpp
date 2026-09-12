/****************************************************************************
** Meta object code from reading C++ file 'ShapeProvider.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../ShapeProvider.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ShapeProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ShapeProvider_t {
    const uint offsetsAndSize[46];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ShapeProvider_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ShapeProvider_t qt_meta_stringdata_ShapeProvider = {
    {
QT_MOC_LITERAL(0, 13), // "ShapeProvider"
QT_MOC_LITERAL(14, 10), // "shapeAdded"
QT_MOC_LITERAL(25, 0), // ""
QT_MOC_LITERAL(26, 5), // "Shape"
QT_MOC_LITERAL(32, 5), // "shape"
QT_MOC_LITERAL(38, 5), // "index"
QT_MOC_LITERAL(44, 11), // "shapesReset"
QT_MOC_LITERAL(56, 13), // "statusMessage"
QT_MOC_LITERAL(70, 4), // "text"
QT_MOC_LITERAL(75, 13), // "repaintNeeded"
QT_MOC_LITERAL(89, 12), // "SetShapeType"
QT_MOC_LITERAL(102, 9), // "ShapeType"
QT_MOC_LITERAL(112, 4), // "type"
QT_MOC_LITERAL(117, 8), // "SetColor"
QT_MOC_LITERAL(126, 5), // "color"
QT_MOC_LITERAL(132, 12), // "SetThickness"
QT_MOC_LITERAL(145, 9), // "thickness"
QT_MOC_LITERAL(155, 8), // "SetLabel"
QT_MOC_LITERAL(164, 5), // "label"
QT_MOC_LITERAL(170, 16), // "CancelActiveDraw"
QT_MOC_LITERAL(187, 17), // "AddFinalizedShape"
QT_MOC_LITERAL(205, 13), // "RemoveShapeAt"
QT_MOC_LITERAL(219, 14) // "ClearAllShapes"

    },
    "ShapeProvider\0shapeAdded\0\0Shape\0shape\0"
    "index\0shapesReset\0statusMessage\0text\0"
    "repaintNeeded\0SetShapeType\0ShapeType\0"
    "type\0SetColor\0color\0SetThickness\0"
    "thickness\0SetLabel\0label\0CancelActiveDraw\0"
    "AddFinalizedShape\0RemoveShapeAt\0"
    "ClearAllShapes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShapeProvider[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   86,    2, 0x06,    1 /* Public */,
       6,    0,   91,    2, 0x06,    4 /* Public */,
       7,    1,   92,    2, 0x06,    5 /* Public */,
       9,    0,   95,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    1,   96,    2, 0x0a,    8 /* Public */,
      13,    1,   99,    2, 0x0a,   10 /* Public */,
      15,    1,  102,    2, 0x0a,   12 /* Public */,
      17,    1,  105,    2, 0x0a,   14 /* Public */,
      19,    0,  108,    2, 0x0a,   16 /* Public */,
      20,    1,  109,    2, 0x0a,   17 /* Public */,
      21,    1,  112,    2, 0x0a,   19 /* Public */,
      22,    0,  115,    2, 0x0a,   21 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::QColor,   14,
    QMetaType::Void, QMetaType::Float,   16,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void,
    QMetaType::Int, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

       0        // eod
};

void ShapeProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        auto *_t = static_cast<ShapeProvider *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->shapeAdded((*reinterpret_cast< const Shape(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->shapesReset(); break;
        case 2: _t->statusMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->repaintNeeded(); break;
        case 4: _t->SetShapeType((*reinterpret_cast< ShapeType(*)>(_a[1]))); break;
        case 5: _t->SetColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 6: _t->SetThickness((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->SetLabel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->CancelActiveDraw(); break;
        case 9: { int _r = _t->AddFinalizedShape((*reinterpret_cast< Shape(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->RemoveShapeAt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->ClearAllShapes(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ShapeProvider::*)(const Shape & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ShapeProvider::shapeAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ShapeProvider::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ShapeProvider::shapesReset)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ShapeProvider::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ShapeProvider::statusMessage)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ShapeProvider::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ShapeProvider::repaintNeeded)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject ShapeProvider::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ShapeProvider.offsetsAndSize,
    qt_meta_data_ShapeProvider,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ShapeProvider_t
, QtPrivate::TypeAndForceComplete<ShapeProvider, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const Shape &, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<ShapeType, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QColor &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<Shape, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ShapeProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShapeProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ShapeProvider.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ShapeProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ShapeProvider::shapeAdded(const Shape & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ShapeProvider::shapesReset()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ShapeProvider::statusMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ShapeProvider::repaintNeeded()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
