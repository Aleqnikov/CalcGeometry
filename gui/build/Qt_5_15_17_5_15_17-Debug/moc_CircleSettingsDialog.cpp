/****************************************************************************
** Meta object code from reading C++ file 'CircleSettingsDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../dialogs/CircleSettingsDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CircleSettingsDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CircleSettingsDialog_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CircleSettingsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CircleSettingsDialog_t qt_meta_stringdata_CircleSettingsDialog = {
    {
QT_MOC_LITERAL(0, 0, 20) // "CircleSettingsDialog"

    },
    "CircleSettingsDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CircleSettingsDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CircleSettingsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject CircleSettingsDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<SettingsDialog::staticMetaObject>(),
    qt_meta_stringdata_CircleSettingsDialog.data,
    qt_meta_data_CircleSettingsDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CircleSettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CircleSettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CircleSettingsDialog.stringdata0))
        return static_cast<void*>(this);
    return SettingsDialog::qt_metacast(_clname);
}

int CircleSettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SettingsDialog::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
