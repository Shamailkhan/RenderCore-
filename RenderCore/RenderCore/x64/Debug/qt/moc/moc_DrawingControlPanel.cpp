/****************************************************************************
** Meta object code from reading C++ file 'DrawingControlPanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../DrawingControlPanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DrawingControlPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DrawingControlPanel_t {
    const uint offsetsAndSize[62];
    char stringdata0[434];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_DrawingControlPanel_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_DrawingControlPanel_t qt_meta_stringdata_DrawingControlPanel = {
    {
QT_MOC_LITERAL(0, 19), // "DrawingControlPanel"
QT_MOC_LITERAL(20, 16), // "shapeTypeChanged"
QT_MOC_LITERAL(37, 0), // ""
QT_MOC_LITERAL(38, 9), // "ShapeType"
QT_MOC_LITERAL(48, 4), // "type"
QT_MOC_LITERAL(53, 12), // "colorChanged"
QT_MOC_LITERAL(66, 5), // "color"
QT_MOC_LITERAL(72, 16), // "thicknessChanged"
QT_MOC_LITERAL(89, 9), // "thickness"
QT_MOC_LITERAL(99, 12), // "labelChanged"
QT_MOC_LITERAL(112, 5), // "label"
QT_MOC_LITERAL(118, 24), // "addShapeFromGeoRequested"
QT_MOC_LITERAL(143, 5), // "Shape"
QT_MOC_LITERAL(149, 5), // "shape"
QT_MOC_LITERAL(155, 20), // "removeShapeRequested"
QT_MOC_LITERAL(176, 5), // "index"
QT_MOC_LITERAL(182, 17), // "clearAllRequested"
QT_MOC_LITERAL(200, 12), // "OnShapeAdded"
QT_MOC_LITERAL(213, 12), // "RefreshTable"
QT_MOC_LITERAL(226, 18), // "std::vector<Shape>"
QT_MOC_LITERAL(245, 6), // "shapes"
QT_MOC_LITERAL(252, 16), // "SetStatusMessage"
QT_MOC_LITERAL(269, 4), // "text"
QT_MOC_LITERAL(274, 23), // "OnShapeTypeComboChanged"
QT_MOC_LITERAL(298, 10), // "comboIndex"
QT_MOC_LITERAL(309, 18), // "OnPickColorClicked"
QT_MOC_LITERAL(328, 17), // "OnAddPointClicked"
QT_MOC_LITERAL(346, 27), // "OnRemovePendingPointClicked"
QT_MOC_LITERAL(374, 17), // "OnAddShapeClicked"
QT_MOC_LITERAL(392, 23), // "OnRemoveSelectedClicked"
QT_MOC_LITERAL(416, 17) // "OnClearAllClicked"

    },
    "DrawingControlPanel\0shapeTypeChanged\0"
    "\0ShapeType\0type\0colorChanged\0color\0"
    "thicknessChanged\0thickness\0labelChanged\0"
    "label\0addShapeFromGeoRequested\0Shape\0"
    "shape\0removeShapeRequested\0index\0"
    "clearAllRequested\0OnShapeAdded\0"
    "RefreshTable\0std::vector<Shape>\0shapes\0"
    "SetStatusMessage\0text\0OnShapeTypeComboChanged\0"
    "comboIndex\0OnPickColorClicked\0"
    "OnAddPointClicked\0OnRemovePendingPointClicked\0"
    "OnAddShapeClicked\0OnRemoveSelectedClicked\0"
    "OnClearAllClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DrawingControlPanel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  116,    2, 0x06,    1 /* Public */,
       5,    1,  119,    2, 0x06,    3 /* Public */,
       7,    1,  122,    2, 0x06,    5 /* Public */,
       9,    1,  125,    2, 0x06,    7 /* Public */,
      11,    1,  128,    2, 0x06,    9 /* Public */,
      14,    1,  131,    2, 0x06,   11 /* Public */,
      16,    0,  134,    2, 0x06,   13 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      17,    2,  135,    2, 0x0a,   14 /* Public */,
      18,    1,  140,    2, 0x0a,   17 /* Public */,
      21,    1,  143,    2, 0x0a,   19 /* Public */,
      23,    1,  146,    2, 0x08,   21 /* Private */,
      25,    0,  149,    2, 0x08,   23 /* Private */,
      26,    0,  150,    2, 0x08,   24 /* Private */,
      27,    0,  151,    2, 0x08,   25 /* Private */,
      28,    0,  152,    2, 0x08,   26 /* Private */,
      29,    0,  153,    2, 0x08,   27 /* Private */,
      30,    0,  154,    2, 0x08,   28 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QColor,    6,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 12, QMetaType::Int,   13,   15,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DrawingControlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        auto *_t = static_cast<DrawingControlPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->shapeTypeChanged((*reinterpret_cast< ShapeType(*)>(_a[1]))); break;
        case 1: _t->colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 2: _t->thicknessChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->labelChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->addShapeFromGeoRequested((*reinterpret_cast< Shape(*)>(_a[1]))); break;
        case 5: _t->removeShapeRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->clearAllRequested(); break;
        case 7: _t->OnShapeAdded((*reinterpret_cast< const Shape(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->RefreshTable((*reinterpret_cast< const std::vector<Shape>(*)>(_a[1]))); break;
        case 9: _t->SetStatusMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->OnShapeTypeComboChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->OnPickColorClicked(); break;
        case 12: _t->OnAddPointClicked(); break;
        case 13: _t->OnRemovePendingPointClicked(); break;
        case 14: _t->OnAddShapeClicked(); break;
        case 15: _t->OnRemoveSelectedClicked(); break;
        case 16: _t->OnClearAllClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DrawingControlPanel::*)(ShapeType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DrawingControlPanel::shapeTypeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DrawingControlPanel::*)(const QColor & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DrawingControlPanel::colorChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DrawingControlPanel::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DrawingControlPanel::thicknessChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DrawingControlPanel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DrawingControlPanel::labelChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DrawingControlPanel::*)(Shape );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DrawingControlPanel::addShapeFromGeoRequested)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DrawingControlPanel::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DrawingControlPanel::removeShapeRequested)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DrawingControlPanel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DrawingControlPanel::clearAllRequested)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject DrawingControlPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DrawingControlPanel.offsetsAndSize,
    qt_meta_data_DrawingControlPanel,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_DrawingControlPanel_t
, QtPrivate::TypeAndForceComplete<DrawingControlPanel, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<ShapeType, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QColor &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Shape, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const Shape &, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const std::vector<Shape> &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *DrawingControlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DrawingControlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DrawingControlPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DrawingControlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void DrawingControlPanel::shapeTypeChanged(ShapeType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DrawingControlPanel::colorChanged(const QColor & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DrawingControlPanel::thicknessChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DrawingControlPanel::labelChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DrawingControlPanel::addShapeFromGeoRequested(Shape _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DrawingControlPanel::removeShapeRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DrawingControlPanel::clearAllRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
