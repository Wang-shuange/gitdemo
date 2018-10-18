/****************************************************************************
** Meta object code from reading C++ file 'loghandle.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../loghandle.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loghandle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_logHandle[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   11,   10,   10, 0x0a,
      52,   38,   10,   10, 0x0a,
      94,   77,   10,   10, 0x0a,
     124,   10,   10,   10, 0x0a,
     136,   11,   10,   10, 0x0a,
     159,  155,   10,   10, 0x0a,
     208,  175,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_logHandle[] = {
    "logHandle\0\0fileName\0newExcel(QString)\0"
    "sheetName,cnt\0appendSheet(QString,int)\0"
    "row,column,value\0setCellValue(int,int,QString)\0"
    "freeExcel()\0saveExcel(QString)\0log\0"
    "LogAdd(QString)\0sn,item,spec,value,res,start,end\0"
    "addTestLog(QString,QString,QString,QString,QString,QString,QString)\0"
};

void logHandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        logHandle *_t = static_cast<logHandle *>(_o);
        switch (_id) {
        case 0: _t->newExcel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->appendSheet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->setCellValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->freeExcel(); break;
        case 4: _t->saveExcel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->LogAdd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->addTestLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData logHandle::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject logHandle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_logHandle,
      qt_meta_data_logHandle, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &logHandle::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *logHandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *logHandle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_logHandle))
        return static_cast<void*>(const_cast< logHandle*>(this));
    return QObject::qt_metacast(_clname);
}

int logHandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
