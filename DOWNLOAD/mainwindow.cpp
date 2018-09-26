#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSettings>
#include<QDebug>
#include<QMessageBox>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QTextCodec>
#include<QAxObject>
#include<QDateTime>
#include <QHeaderView>
#include <QDomDocument>
#include<QTcpSocket>
#include<QIcon>
#include<QThread>
#include<QFileDialog>
#include<QRegExp>
#include<QReadLocker>
#include"mythread.h"
#include<string>
QReadWriteLock lock;
QString cmd;
QMutex mutex;
QStringList testLog;
MyThread *my=new MyThread();
QTimer*timer=new QTimer();
void MainWindow::downloadConfig()
{

    QStringList baudList,parityList,echoList;
    baudList.append("256000");
    baudList.append("115200");
    baudList.append("9600");

    parityList.append("EVEN");
    parityList.append("NONE");
    parityList.append("ODD");
    echoList.append("OFF");
    echoList.append("ON");
    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_ODD,STOP_1,FLOW_OFF,500};//定义一个结构体，用来存放串口各个参数
    QString str=ui->portNameComboBox->currentText();
    QString s=str.mid(3);
    int com_value=s.trimmed().toInt();
    QString com_path;
    if(com_value>=10)
        com_path="\\\\.\\";
    else
        com_path="\\";
    com_path+=str;
    myCom = new Win_QextSerialPort(com_path,myComSetting,QextSerialBase::EventDriven);
    if(myCom->isOpen())
    {
        qDebug()<<"com is opened";
        myCom->close();
    }
    ui->baud_rate_comboBox->addItems(baudList);
    ui->data_bit_lineEdit->setText("8");
    ui->data_bit_lineEdit->setDisabled(true);
    ui->parity_comboBox->addItems(parityList);
    ui->echo_comboBox->addItems(echoList);
    QString path =  QDir::currentPath()+"/APP.hex"; //文件名可从此处修改
    ui->firmware_lineEdit->setText(path);
    QRegExp regExp("[H][S][A-Z]{1}\\d[A-Z]{2}\\d{10}");//使用正则表达式判断SN号是否正确
    ui->SN_lineEdit->setValidator(new QRegExpValidator(regExp, this));
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    downloadConfig();
    connect(ui->portNameComboBox, SIGNAL(clicked()), this, SLOT(getPadIds()));
    this->testCount=0;
    ui->result_label->setAlignment(Qt::AlignCenter);
    ui->result_label->setFont(QFont("Roman times",50));
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd-hh-mm-ss").left(10); //设置显示格式并截取当前日期；
    qDebug()<<str;
    QString file_txt =  QDir::currentPath()+"/"+str+"_total_log.txt"; //文件名可从此处修改
    file=new QFile(file_txt);
    if(!file->open(QIODevice::ReadWrite | QIODevice::Text|QIODevice::Append)) //以文本文式写入
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    this->showNormal();
    ui->statusBar->showMessage("Software rev:03              Author:王拴阁               Cell:87249");
    ui->log_tableWidget->setColumnCount(7);
    for(int i=0;i<ui->log_tableWidget->columnCount();++i)
        ui->log_tableWidget->setColumnWidth(i,90);
    ui->log_tableWidget->setWindowTitle(tr("test log"));
    ui->log_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<ui->log_tableWidget->columnCount();++i)
    {
        QTableWidgetItem *columnHeaderItem0 =ui->log_tableWidget->horizontalHeaderItem(i); //获得水平方向表头的Item对象
        QFont f("Helvetica");
        columnHeaderItem0->setFont(f); //设置字体
        columnHeaderItem0->setBackgroundColor(QColor(20,0,255)); //设置单元格背景颜色
        columnHeaderItem0->setTextColor(QColor(0,0,255)); //设置文字颜色
    }

    ui->log_tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->log_tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->log_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置为自动填充方式
    ui->log_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    ui->log_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//显示滚动条

    QFont ft("Arial", 48,80);
    ft.setPointSize(48);
    ui->result_label->setFont(ft);
    ui->result_label->setFrameShape (QFrame::Box);
    ui->result_label->setStyleSheet("border-width: 20px;border-style: solid;border-color: rgb(255, 0, 0);");
    ui->result_label->setText(tr("WAITING"));
    ui->result_label->setStyleSheet("background-color:purple;color:white;");
    this->getAvailableCom();
}
MainWindow::~MainWindow()
{
    file->close();
    if(myCom->isOpen())
    {
        qDebug()<<"com is opened";
        myCom->close();
    }else
        qDebug()<<"com is closed";
    delete ui;
}

QString MainWindow::getcomm(int index,QString keyorvalue)
{
    QString commresult;
    if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),0,KEY_READ,&hKey)!=0)
    {
        QString error="Cannotopenregedit!";//无法打开注册表时返回error
        return error;
    }//if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&hKey)!=0)
    QString keymessage;//键名
    QString message;
    QString valuemessage;//键值
    indexnum = index;//要读取键值的索引号
    keysize = sizeof(keyname);
    valuesize = sizeof(keyvalue);
    if(::RegEnumValue(hKey,indexnum,keyname,&keysize,0,&type,(BYTE*)keyvalue,&valuesize)==0)//列举键名和值
    {
        for(int i=0;i<keysize;i++)
        {
            message = keyname[i];
            keymessage.append(message);
        }
        for(int j=0;j<valuesize;j++) //读取键名
        {
            if(keyvalue[j]!=0x00)
            {
                valuemessage.append(keyvalue[j]);
            }
        }//for(intj=0;j<valuesize;j++)读取键值
        if(keyorvalue=="key")
        {
            commresult=keymessage;
        }
        if(keyorvalue=="value")
        {
            commresult=valuemessage;
        }
    }
    else
    {
        commresult="nokey";
    }//if(::RegEnumValue(hKey,indexnum,keyname,&keysize,0,&type,(BYTE*)keyvalue,&valuesize)==0)列举键名和值
    return commresult;
    ::RegCloseKey(hKey);//关闭注册表
}
void MainWindow::sendTestCommand(QString command)
{
    qDebug()<<"send:"<<command.toAscii().constData();
    myCom->write(command.toAscii().constData());
    ui->textBrowser->insertPlainText("<<<---");
    ui->textBrowser->insertPlainText(command.toAscii().constData());
    ui->textBrowser->insertPlainText("\r\n");
}

void MainWindow::appTestLogShow(QString sn,QString item,QString spec,QString val,QString res,QString start,QString end)
{
    int RowCont=ui->log_tableWidget->rowCount();
    ui->log_tableWidget->insertRow(RowCont);//增加一行
    ui->log_tableWidget->setItem(RowCont,0,new QTableWidgetItem(sn));
    ui->log_tableWidget->setItem(RowCont,1,new QTableWidgetItem(item));
    ui->log_tableWidget->setItem(RowCont,2,new QTableWidgetItem(spec));
    ui->log_tableWidget->setItem(RowCont,3,new QTableWidgetItem(val));
    ui->log_tableWidget->setItem(RowCont,4,new QTableWidgetItem(res));
    ui->log_tableWidget->setItem(RowCont,5,new QTableWidgetItem(start));
    ui->log_tableWidget->setItem(RowCont,6,new QTableWidgetItem(end));
    logHandle *la=new logHandle();
    la->addTestLog(sn,item,spec,val,res,start,end);
}

TCHAR * MainWindow::CharToWchar(const QString &str)
{
    QByteArray ba = str.toUtf8();
    char *data = ba.data(); //以上两步不能直接简化为“char *data = str.toUtf8().data();”
    int charLen = strlen(data);
    int len = MultiByteToWideChar(CP_ACP, 0, data, charLen, NULL, 0);
    TCHAR *buf = new TCHAR[len + 1];
    MultiByteToWideChar(CP_ACP, 0, data, charLen, buf, len);
    buf[len] = '\0';
    return buf;
}
long MainWindow::getFirmwareSize(QString firmware)
{
    QFile file(firmware);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return 1;
    }

    QTextStream in(&file);
    QString myText = in.readAll();
    qDebug() << myText;
    long siz=file.size();
    qDebug()<<"#firmware size:"<<siz;
    file.close();
    return siz;
}

void MainWindow::handleTimeout()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd-hh:mm:ss"); //设置显示格式并截取当前日期；
    str="######Firmware downloading......"+str.right(5);
    ui->textBrowser->append(str);

}
void MainWindow::downloadStartSlot(QString s)
{
    qDebug()<<"sub thread send:"<<s;
    ui->textBrowser->append("######FIRMWARE DOWNLOAD START######");
    ui->start_pushButton->setEnabled(false);
    ui->result_label->setText("DOWNLOADING");
    ui->result_label->setStyleSheet("background-color:orange;color:white;");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->log_tableWidget->setRowCount(0);
    ui->log_tableWidget->clearContents();
    this->test_result=true;
    this->isDownloadTested=false;
    timer->start(1000);
}
void MainWindow::downloadFinishSlot()
{
    ui->textBrowser->append("######FIRMWARE DOWNLOAD FINISHED######");
    ui->start_pushButton->setEnabled(true);
    timer->stop();


}
void MainWindow::logReadSlot(QString s)
{
    QString ret;
    qDebug()<<"###received:"<<s;
    ui->textBrowser->append("######READ LOG INFO######");
    if(s=="[KO]")
    {
        ui->textBrowser->append("DOWNLOAD FAIL");
        this->test_result=false;
    }
    else if(s=="")
        ui->textBrowser->append("NO INFO PASSED");
    else
    {
        ui->textBrowser->append("DOWNLOAD PASS");
        this->test_result==true;
    }
    if(this->test_result==true)
    {
        this->testCount=0;
        ui->result_label->setStyleSheet("background-color:green;color:white;");
        ui->result_label->setText(tr("PASS"));
    }else
    {
        this->testCount=0;
        ui->result_label->setStyleSheet("background-color:red;color:white;");
        ui->result_label->setText(tr("FAIL"));
    }

    this->test_result?ret="PASS":ret="FAIL";

    QString val=(ret=="PASS"?"DOWNLOAD SUCCESS":"DOWNLOAD FAIL");
    QDateTime endTime = QDateTime::currentDateTime();//获取系统现在的时间
    this->end = endTime.toString("yyyy-MM-dd-hh:mm:ss"); //设置显示格式并截取当前日期；
    this->appTestLogShow(ui->SN_lineEdit->text(),"FIRMWARE DOWNLOAD","DOWNLOAD SUCCESS/FAIL",val,ret,start,end);
}

void MainWindow::on_start_pushButton_clicked()
{
    if(ui->SN_lineEdit->text().length()!=16)
    {
        int ret=QMessageBox::warning(this, "warning", "SN长度不正确!", QMessageBox::Apply , QMessageBox::Apply);
        if(QMessageBox::Apply == ret)
        {
            return;
        }
    }
    if(ui->firmware_lineEdit->text().length()<=0)
    {
        int ret=QMessageBox::warning(this, "warning", "固件路径为空!", QMessageBox::Apply , QMessageBox::Apply);
        if(QMessageBox::Apply == ret)
        {
            return;
        }
    }
    if(!this->isDownloadTested)
    {

        QDateTime startTime = QDateTime::currentDateTime();//获取系统现在的时间
        this->start = startTime.toString("yyyy-MM-dd-hh:mm:ss"); //设置显示格式并截取当前日期；
        struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_ODD,STOP_1,FLOW_OFF,500};//定义一个结构体，用来存放串口各个参数
        QString str=ui->portNameComboBox->currentText();
        QString s=str.mid(3);
        int com_value=s.trimmed().toInt();
        QString com_path;
        if(com_value>=10)
            com_path="\\\\.\\";
        else
            com_path="\\";
        com_path+=str;
        myCom = new Win_QextSerialPort(com_path,myComSetting,QextSerialBase::EventDriven);
        if(myCom->isOpen())
        {
            qDebug()<<"com is opened";
            myCom->close();
        }else
            qDebug()<<"com is closed";

        QThread *thread = new QThread;
        MyThread *my=new MyThread();
        logHandle *lh=new logHandle();
        connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
        my->moveToThread(thread);
        lh->moveToThread(thread);
        mutex.lock();
        thread->start();
        connect(thread, SIGNAL(started()), my, SLOT(download_Save()));
        connect(this, SIGNAL(startDownload()), my, SLOT(download_Save()));
        //  connect(this, SIGNAL(downloadNotify(QString)), my, SLOT(download_infoSlot(QString)),Qt::BlockingQueuedConnection);
        QString send="STMFlashLoader.exe -c --pn ";
        send+=s;
        send+=" --br ";
        send+=ui->baud_rate_comboBox->currentText();
        send+=" --db ";
        send+=ui->data_bit_lineEdit->text();
        send+=" --pr ";
        send+=ui->parity_comboBox->currentText();
        send+=" --sb 1 --ec ";
        send+=ui->echo_comboBox->currentText();
        send+="  --to ";
        send+=ui->time_out_comboBox->currentText();
        send+="   -Dtr --Hi -Rts --Lo  -i ";
        send+=ui->target_mcu_comboBox->currentText();
        send+=" -e --all -d --a @08000000 --fn ";
        send+=ui->firmware_lineEdit->text();
        cmd=send;
     //   connect(this, SIGNAL(logSendNotify(QString,QString,QString,QString,QString,QString,QString)), lh, SLOT(addTestLog(QString,QString ,QString,QString,QString,QString,QString)),Qt::BlockingQueuedConnection);
        connect(my, SIGNAL(downloadStartNotify(QString)), this, SLOT(downloadStartSlot(QString)),Qt::BlockingQueuedConnection);
        connect(my, SIGNAL(downloadFinishNotify()), this, SLOT(downloadFinishSlot()));
        connect(my, SIGNAL(logReadNotify(QString)), this, SLOT(logReadSlot(QString)),Qt::BlockingQueuedConnection);
      //  emit logSendNotify("HELLO","1","2","3","4","5","6");
        thread->quit();
        mutex.unlock();
        this->isDownloadTested=true;
        this->testCount++;
    }
}

void MainWindow::parseCommand(QString st)
{
    if(st.indexOf("Data Send:")!=-1&&st.indexOf("*"))
    {
        QString t= st.mid(st.indexOf("Data Send:"),st.indexOf('*'));
        qDebug()<<"$sensor:"<<t;

    }
}

void MainWindow::getAvailableCom()
{
    QString path="HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
    QSettings * settings = new QSettings(path,QSettings::NativeFormat);
    QStringList key = settings->allKeys();
    int num=(int)key.size();
    QString value;
    ui->portNameComboBox->clear();
    for(int i=0;i<num;i++)
    {
        value = getcomm(i,"value");
        qDebug()<<i<<value;
        if(ui->portNameComboBox->findText(value)==-1)
            ui->portNameComboBox->addItem(value);
    }
}
void MainWindow::getPadIds()
{
    this->getAvailableCom();
}
void MainWindow::on_file_select_pushButton_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);

    fileDialog->setWindowTitle(tr("打开固件文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*.hex *.bin *.s19 )"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
        ui->firmware_lineEdit->setText(fileNames.at(0));
    }
}
