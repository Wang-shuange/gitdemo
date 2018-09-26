/****************************************************************************
** Meta object code from reading C++ file 'confighandle.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../confighandle.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'confighandle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_configHandle[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   14,   13,   13, 0x0a,
      94,   85,   13,   13, 0x0a,
     112,   85,   13,   13, 0x0a,
     145,  132,   13,   13, 0x0a,
     169,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_configHandle[] = {
    "configHandle\0\0filename,rmt_name,ipa,ipb\0"
    "add_xmlnode(QString,QString,QString,QString)\0"
    "filename\0read_xml(QString)\0"
    "create_xml(QString)\0opt,filename\0"
    "do_xml(QString,QString)\0read()\0"
};

void configHandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        configHandle *_t = static_cast<configHandle *>(_o);
        switch (_id) {
        case 0: _t->add_xmlnode((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->read_xml((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->create_xml((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->do_xml((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->read(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData configHandle::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject configHandle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_configHandle,
      qt_meta_data_configHandle, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &configHandle::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *configHandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *configHandle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_configHandle))
        return static_cast<void*>(const_cast< configHandle*>(this));
    return QObject::qt_metacast(_clname);
}

int configHandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
