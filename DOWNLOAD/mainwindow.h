#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "win_qextserialport.h"
#include<QAxObject>
#include<QDir>
#include <QTextStream>
#include <QDataStream>
#include<QTableWidgetItem>
#include<QTimer>
#include "libmodbus/modbus.h"
#include"libmodbus/modbus-private.h"
#include<QTcpServer>
#include"loghandle.h"
extern int count;
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:QString getcomm(int index,QString keyorvalue);
signals:
    void downloadNotify(QString);
    void startDownload();
    void logSendNotify(QString,QString,QString,QString,QString,QString,QString);
private slots:
    void on_start_pushButton_clicked();
    void on_file_select_pushButton_clicked();
    void on_actionAbout_triggered();

public:
    void parseCommand(QString st);
    void sendTestCommand(QString command);
    void appTestLogShow(QString sn, QString item, QString spec, QString val, QString res, QString start, QString end);
    void getAvailableCom();
    void downloadConfig();
    void download_Save();
    TCHAR *CharToWchar(const QString &str);
    long getFirmwareSize(QString firmware);
    void killFlashloader();
public slots:

    void getPadIds();
    void handleTimeout();
    void downloadStartSlot(QString s);
    void downloadFinishSlot();
    void logReadSlot(QString s);
private:
    Ui::MainWindow *ui;
    QStringList m_listcomboName;
    HKEY hKey;
    LPCWSTR subkey;
    wchar_t keyname[256];
    char keyvalue[256];
    DWORD keysize,type,valuesize;
    int indexnum;
    Win_QextSerialPort *myCom;
    QFile *file;
    QString SN;
    QString testItem;
    QString Spec;
    QString Value;
    QString Result;
    QString start;
    QString end;
    QString fileNametmp;
    int timer_count;
    int testCount;
    bool test_result=false;
    QByteArray temp;
    bool isAllTestFinished=false;
    bool isDownloadTested=false;

};

#endif // MAINWINDOW_H
