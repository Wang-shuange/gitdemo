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

private slots:
    void readMyCom();
    void on_start_pushButton_clicked();
    void on_actionAbout_triggered();

public:
    void newExcel(const QString &fileName);//1./打开
    void appendSheet(const QString &sheetName,int cnt);    //2.增加1个Worksheet
    void setCellValue(int row, int column, const QString &value);    //3.向Excel单元格中写入数据
    void saveExcel(const QString &fileName);    //4.保存Excel
    void  freeExcel();   //5.释放Excel
    void totalLogAdd(QString log);
    void parseCommand(QString st);
      void addTestLog(QString sn, QString item, QString spec, QString value, QString res, QString start, QString end);
    void sendTestCommand(QString command);

    void appTestLogShow(QString sn, QString item, QString spec, QString val, QString res, QString start, QString end);
    QString ethernetTest(QString st);
    void ethernet_test(QString input);
    QString eth_macWrite(QString st);
    void read_eth_mac(QString input);
    void write_eth_mac(QString input, QString mac);
    QString eth_macRead(QString input, QString &st);
    void InitSocket();
    void SendData(QString sendMsg);
    void doRtuQuery(int &regis);
    void Test(QString testItem, QString spec, QString buf);
    void Delay_MSec_Suspend(unsigned int msec);
    void getAvailableCom();
public slots:
    void ServerReadData();
    void ServerNewConnection();
    void sServerDisConnection();
    void getPadIds();
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

    QAxObject *pApplication;
    QAxObject *pWorkBooks;
    QAxObject *pWorkBook;
    QAxObject * pSheets;
    QAxObject * pSheet;
    QFile *file;
    QString SN;
    QString testItem;
    QString Spec;
    QString Value;
    QString Result;
    QString start;
    QString end;
    int last_row,last_column;
    QString fileNametmp;
    QTimer *timer;
    int timer_count;
    int testCount;
    bool isEthConnectTested=false;
    bool isEthReceiveTested=false;
    bool isEthSendTested=false;
    bool isEthDisconnectTested=false;
    bool isRs485AddrWrited=false;
    bool isRs485CommTested=false;


    bool test_result=false;
    QTcpServer *mp_TCPServer;
    QTcpSocket *mp_TCPSocket;
    QString  rece;
    QByteArray temp;
    bool isHermesReceived=false;
    bool isHermesSend=false;
    bool isHermesSendCompleted=false;
    bool isHermesReceivedCompleted=false;
    int status_eth_con;
    bool isAllTestFinished=false;
    int test_count=0;
    bool hasNewConnecting=false;
};

#endif // MAINWINDOW_H
