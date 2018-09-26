/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      24,   11,   11,   11, 0x08,
      54,   11,   11,   11, 0x08,
     101,   84,   11,   11, 0x08,
     187,  176,   11,   11, 0x08,
     229,  176,   11,   11, 0x08,
     321,  269,   11,   11, 0x08,
     382,  376,   11,   11, 0x08,
     424,  376,   11,   11, 0x08,
     464,   11,   11,   11, 0x08,
     506,   11,   11,   11, 0x08,
     530,   11,   11,   11, 0x08,
     558,   11,   11,   11, 0x08,
     587,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0readMyCom()\0"
    "on_start_pushButton_clicked()\0"
    "on_excel_pushButton_clicked()\0"
    "current,previous\0"
    "on_log_tableWidget_currentItemChanged(QTableWidgetItem*,QTableWidgetIt"
    "em*)\0"
    "row,column\0on_log_tableWidget_cellActivated(int,int)\0"
    "on_log_tableWidget_cellChanged(int,int)\0"
    "currentRow,currentColumn,previousRow,previousColumn\0"
    "on_log_tableWidget_currentCellChanged(int,int,int,int)\0"
    "index\0on_log_tableWidget_activated(QModelIndex)\0"
    "on_log_tableWidget_entered(QModelIndex)\0"
    "on_log_tableWidget_itemSelectionChanged()\0"
    "on_pushButton_clicked()\0"
    "on_ini_pushButton_clicked()\0"
    "on_read_pushButton_clicked()\0processinc()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->readMyCom(); break;
        case 1: _t->on_start_pushButton_clicked(); break;
        //case 2: _t->on_excel_pushButton_clicked(); break;
        case 3: _t->on_log_tableWidget_currentItemChanged((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTableWidgetItem*(*)>(_a[2]))); break;
        //case 4: _t->on_log_tableWidget_cellActivated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        //case 5: _t->on_log_tableWidget_cellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        //case 6: _t->on_log_tableWidget_currentCellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 7: _t->on_log_tableWidget_activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        //case 8: _t->on_log_tableWidget_entered((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->on_log_tableWidget_itemSelectionChanged(); break;
        //case 10: _t->on_pushButton_clicked(); break;
        //case 11: _t->on_ini_pushButton_clicked(); break;
        //case 12: _t->on_read_pushButton_clicked(); break;
        case 13: _t->processinc(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
