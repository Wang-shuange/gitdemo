#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSettings>
#include<QDebug>
#include<QMessageBox>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QTextCodec>
#include<QDateTime>
#include <QHeaderView>
#include<QTcpSocket>
#include<QIcon>
#include<QThread>
#include<windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->portNameComboBox, SIGNAL(clicked()), this, SLOT(getPadIds()));
    ui->test_progressBar->setValue(0.0);
    ui->virtual_lineEdit->setText("COM2");
    ui->serverIP_lineEdit->setText("192.168.1.110");
    ui->srv_port_lineEdit->setText("8443");
    ui->resp_to_lineEdit->setMaxLength(4);
    ui->resp_to_lineEdit->setText("3000");
    //    ui->SN_lineEdit->setMaxLength(16);

    QRegExp regExp("[H][S][A-Z]{1}\\d[A-Z]{2}\\d{10}");//使用正则表达式判断SN号是否正确
    ui->SN_lineEdit->setValidator(new QRegExpValidator(regExp, this));
    this->testCount=0;
    ui->test_progressBar->setRange(0,4);
    ui->result_label->setAlignment(Qt::AlignCenter);
    ui->result_label->setFont(QFont("Roman times",50));
    this->last_row=0;
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
    ui->statusBar->showMessage("Software rev:1.0.0.5            Author:王拴阁               Cell:87249");
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
    QDateTime startTime = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = startTime.toString("yyyy-MM-dd-hh-mm-ss"); //设置显示格式并截取当前日期；
    int RowCont=ui->log_tableWidget->rowCount();
    ui->log_tableWidget->insertRow(RowCont);//增加一行
    ui->log_tableWidget->setItem(RowCont,0,new QTableWidgetItem(sn));
    ui->log_tableWidget->setItem(RowCont,1,new QTableWidgetItem(item));
    ui->log_tableWidget->setItem(RowCont,2,new QTableWidgetItem(spec));
    ui->log_tableWidget->setItem(RowCont,3,new QTableWidgetItem(val));
    ui->log_tableWidget->setItem(RowCont,4,new QTableWidgetItem(res));
    ui->log_tableWidget->setItem(RowCont,5,new QTableWidgetItem(str));
    QDateTime endTime = QDateTime::currentDateTime();//获取系统现在的时间
    QString endstr = endTime.toString("yyyy-MM-dd-hh:mm:ss"); //设置显示格式并截取当前日期；
    ui->log_tableWidget->setItem(RowCont,6,new QTableWidgetItem(endstr));
    addTestLog(sn,item,spec,val,res,str,endstr);//save to excel log
}

void MainWindow::readMyCom() //读串口函数
{
    temp = myCom->readAll(); //读取串口缓冲区的所有数据给临时变量temp
    ui->textBrowser->insertPlainText(temp); //将串口的数据显示在窗口的文本浏览器中
    totalLogAdd(temp);
    ui->textBrowser->moveCursor(QTextCursor::End);
}
void MainWindow::on_start_pushButton_clicked()
{

    this->isEthReceiveTested=false;
    this->isEthSendTested=false;
    this->isRs485AddrWrited=false;
    this->isRs485CommTested=false;
    ui->log_tableWidget->setRowCount(0);
    ui->log_tableWidget->clearContents();
    ui->textBrowser->append(this->rece);
    if(ui->SN_lineEdit->text().isEmpty())
    {
        int ret=QMessageBox::warning(this, "warning", "SN不能为空!", QMessageBox::Apply , QMessageBox::Apply);
        if(QMessageBox::Apply == ret)
        {
            return;
        }
    }
    if(ui->addr_lineEdit->text().isEmpty())
    {
        int ret=QMessageBox::warning(this, "warning", "设备地址不能为空!", QMessageBox::Apply , QMessageBox::Apply);
        if(QMessageBox::Apply == ret)
        {
            return;
        }
    }
    ui->result_label->setText("TESTING");
    ui->result_label->setStyleSheet("background-color:orange;color:white;");
    ui->textBrowser->moveCursor(QTextCursor::End);
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
    if(!myCom ->open(QIODevice::ReadWrite))   //定义串口对象，并传递参数，在构造函数里对其进行初始化 //以可读写方式打开串口
    {
        ui->start_pushButton->setEnabled(true);
        int ret=QMessageBox::warning(this, "warning", "串口无法打开！", QMessageBox::Apply , QMessageBox::Apply);
        if(QMessageBox::Apply == ret)
        {
            return;
        }
    }else
    {
        myCom->setBaudRate(BAUD115200);
        myCom->setDataBits(DATA_8);
        myCom->setParity(PAR_ODD);
        myCom->stopBits();
        myCom->setFlowControl(FLOW_OFF);
        myCom->setTimeout(500);
        ui->start_pushButton->setEnabled(false);
        connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));   //信号和槽函数关联，当串口缓冲区有数据时，进行读串口操作
    }
    this->test_result=true;
    //    QTime t;
    //    t.start();
    //    while(t.elapsed()<5000)
    //        QCoreApplication::processEvents();
    //    qDebug()<<"wait========";
    this->hasNewConnecting=false;
    InitSocket();
    QTime t1;
    t1.start();
    while(t1.elapsed()<10000)
        QCoreApplication::processEvents();
    qDebug()<<"wait========";
    qDebug()<<"###isHermesReceivedCompleted:"<<this->isHermesReceivedCompleted;
    qDebug()<<"###isHermesSendCompleted:"<<this->isHermesSendCompleted;
    qDebug()<<"###isHermesReceived:"<<this->isHermesReceived;
    qDebug()<<"###isHermesSend:"<<this->isHermesSend;
    if((this->isHermesReceivedCompleted==false)&&(this->isHermesSendCompleted==false))
    {
        this->isHermesReceived=false;
        this->isHermesSend=false;
    }
    Test("eth_send_test","ETHERNET SEND TEST PASS/FAIL","");
    ui->test_progressBar->setValue(testCount);
    if(this->isEthSendTested)
    {
        Test("eth_receive_test","ETHERNET RECEIVE TEST PASS/FAIL","");
        ui->test_progressBar->setValue(testCount);
    }
    if(this->isEthReceiveTested)
    {
        Test("rs485_addr_write","RS485 ADDR WRITE OK/FAIL",temp);
        ui->test_progressBar->setValue(testCount);
    }
    if(this->isRs485AddrWrited)
    {
        Test("rs485_communication_test","RS485 COMMUNICATION TEST PASS/FAIL",temp);
        ui->test_progressBar->setValue(testCount);
    }
    qDebug()<<"test count:"<<this->testCount<<endl;
    if(this->testCount>=4)
    {
        this->isHermesReceivedCompleted=false;
        this->isHermesSendCompleted=false;
        ui->test_progressBar->setValue(0);
        qDebug()<<"$test result:"<<this->test_result;
        if(this->test_result)
        {
            ui->result_label->setStyleSheet("background-color:green;color:white;");
            ui->result_label->setText(tr("PASS"));

        }else
        {
            ui->result_label->setStyleSheet("background-color:red;color:white;");
            ui->result_label->setText(tr("FAIL"));
        }
        ui->start_pushButton->setEnabled(true);
        myCom->close();
        this->testCount=0;
        if(this->hasNewConnecting)
        {
            if(mp_TCPSocket!=NULL)
            {
                delete mp_TCPSocket;
                qDebug()<<" --->delete mp_TCPSocket";
            }
        }
        if(this->mp_TCPServer->isListening())
        {
            qDebug()<<"--->listening";
            this->mp_TCPServer->close();
            qDebug()<<" --->close listening";
        }
        if(mp_TCPServer!=NULL)
        {
            delete mp_TCPServer;
            qDebug()<<" --->delete mp_TCPServer";
        }
        this->rece="";
        if(!this->hasNewConnecting)
        {
            ui->textBrowser->append(tr("--->请检查网线是否连接正确，然后重启产品，绿灯闪烁后重新测试！"));
        }
        return;
    }
}
//void MainWindow::on_start_pushButton_clicked()
//{
//    this->isEthReceiveTested=false;
//    this->isEthSendTested=false;
//    this->isRs485AddrWrited=false;
//    this->isRs485CommTested=false;

//    this->test_result=true;

//    Test("eth_send_test","0 or 1",temp);

//    if(this->isEthSendTested)
//        Test("eth_receive_test","0 or 1",temp);

//    ui->log_tableWidget->setRowCount(0);
//    ui->log_tableWidget->clearContents();
//    if(ui->SN_lineEdit->text().isEmpty())
//    {
//        int ret=QMessageBox::warning(this, "warning", "SN不能为空!", QMessageBox::Apply , QMessageBox::Apply);
//        if(QMessageBox::Apply == ret)
//        {
//            return;
//        }
//    }
//    if(ui->addr_lineEdit->text().isEmpty())
//    {
//        int ret=QMessageBox::warning(this, "warning", "设备地址不能为空!", QMessageBox::Apply , QMessageBox::Apply);
//        if(QMessageBox::Apply == ret)
//        {
//            return;
//        }
//    }
//    ui->result_label->setText("TESTING");
//    ui->result_label->setStyleSheet("background-color:orange;color:white;");
//    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_ODD,STOP_1,FLOW_OFF,500};//定义一个结构体，用来存放串口各个参数
//    QString str=ui->portNameComboBox->currentText();
//    QString s=str.mid(3);
//    int com_value=s.trimmed().toInt();
//    QString com_path;
//    if(com_value>=10)
//        com_path="\\\\.\\";
//    else
//        com_path="\\";
//    com_path+=str;
//    myCom = new Win_QextSerialPort(com_path,myComSetting,QextSerialBase::EventDriven);
//    if(!myCom ->open(QIODevice::ReadWrite))   //定义串口对象，并传递参数，在构造函数里对其进行初始化 //以可读写方式打开串口
//    {

//        ui->start_pushButton->setEnabled(true);
//        int ret=QMessageBox::warning(this, "warning", "串口无法打开！", QMessageBox::Apply , QMessageBox::Apply);
//        if(QMessageBox::Apply == ret)
//        {
//            return;
//        }

//    }else
//    {
//        myCom->setBaudRate(BAUD115200);
//        myCom->setDataBits(DATA_8);
//        myCom->setParity(PAR_ODD);
//        myCom->stopBits();
//        myCom->setFlowControl(FLOW_OFF);
//        myCom->setTimeout(500);
//        connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));   //信号和槽函数关联，当串口缓冲区有数据时，进行读串口操作
//        ui->start_pushButton->setEnabled(false);
//        //        InitSocket();
//        //         Sleep(8000);
//    }

//}
//1.新建一个excel
void MainWindow::newExcel(const QString &fileName)
{        HRESULT r = OleInitialize(0);
         CoInitialize(0);
              if (r != S_OK && r != S_FALSE) {
                  qWarning("Qt: Could not initialize OLE (error %x)\n", (unsigned int)r);
              }
              pApplication = new QAxObject("Excel.Application");
                   if(pApplication == NULL){
                       qWarning("pApplication\n");return;
                   }
                   pApplication->dynamicCall("SetVisible(bool)", false);//false不显示窗体
                        pApplication->setProperty("DisplayAlerts", false);//不显示任何警告信息。
                             pWorkBooks = pApplication->querySubObject("Workbooks");
                                  QFile file(fileName);
                                       if (file.exists())
                                       {
                                           pWorkBook = pWorkBooks->querySubObject("Open(const QString &)", fileName);
                                       }
                                       else
                                       {
                                           pWorkBooks->dynamicCall("Add");
                                           pWorkBook = pApplication->querySubObject("ActiveWorkBook");
                                       }
                                            pSheets = pWorkBook->querySubObject("Sheets");
                                                 pSheet = pSheets->querySubObject("Item(int)", 1);
}
//2.增加1个Worksheet
void MainWindow::appendSheet(const QString &sheetName,int cnt)
{
    QAxObject *pLastSheet = pSheets->querySubObject("Item(int)", cnt);
    pSheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
    pSheet = pSheets->querySubObject("Item(int)", cnt);
    pLastSheet->dynamicCall("Move(QVariant)", pSheet->asVariant());
    pSheet->setProperty("Name", sheetName);
}
//3.向Excel单元格中写入数据
void MainWindow::setCellValue(int row, int column, const QString &value)
{
    QAxObject *pRange = pSheet->querySubObject("Cells(int,int)", row, column);
    pRange->dynamicCall("Value", value);
}
//4.保存Excel
void MainWindow::saveExcel(const QString &fileName)
{
    pWorkBook->dynamicCall("SaveAs(const QString &)",
                           QDir::toNativeSeparators(fileName));
}
//5.释放Excel
void  MainWindow::freeExcel()
{
    if (pApplication != NULL)
    {
        pApplication->dynamicCall("Quit()");
        delete pApplication;
        pApplication = NULL;
    }
}

void MainWindow::totalLogAdd(QString log)
{
    QTextStream out(file);
    out << log << endl;

}
void MainWindow::parseCommand(QString st)
{
    if(st.indexOf("Data Send:")!=-1&&st.indexOf("*"))
    {
        QString t= st.mid(st.indexOf("Data Send:"),st.indexOf('*'));
        qDebug()<<"$sensor:"<<t;

    }
}
QString MainWindow::eth_macWrite(QString st)
{
    if(st.trimmed().toUpper().indexOf("ETHERNET_MAC#")!=-1&&st.trimmed().toUpper().indexOf("*")!=-1)
    {
        qDebug()<<"ethernet mac write success";
        return "PASS";
    }
    return "NA";
}
QString MainWindow::eth_macRead(QString input,QString& st)
{
    qDebug()<<"@@@received string:"<<input;
    //ETHERNET_MAC#0008DC22FA6F*
    if(input.trimmed().toUpper().indexOf("ETHERNET_MAC#")!=-1&&input.trimmed().toUpper().indexOf("*")!=-1)
    {
        qDebug()<<"$ mac raw input:"<<input;
        st= input.mid(input.indexOf("ETHERNET_MAC#")+strlen("ETHERNET_MAC#"),12);
        qDebug()<<"$eth read mac:"<<st;
        return "PASS";
    }
    return "NA";
}

void MainWindow::addTestLog(QString sn,QString item,QString spec,QString value,QString res,QString start,QString end)
{
    OleInitialize(0);
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd-hh-mm-ss").left(10); //设置显示格式并截取当前日期；
    qDebug()<<str;
    QString fileNametmp =  QDir::currentPath()+"/"+str+"_test_log.xlsx"; //文件名可从此处修改
    QDir::toNativeSeparators(fileNametmp);
    newExcel(fileNametmp);
    setCellValue(1, 1,"SN");
    setCellValue(1, 2,"TestItem");
    setCellValue(1, 3,"Spec");
    setCellValue(1, 4,"Value");
    setCellValue(1, 5,"Result");
    setCellValue(1, 6,"Start");
    setCellValue(1, 7,"End");
    QAxObject * usedrange = pSheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    int intRows = rows->property("Count").toInt();
    qDebug()<<"total_row"<<intRows;
    last_row=intRows;
    qDebug()<<"last row:"<<last_row;
    setCellValue(last_row+1, 1,sn);
    setCellValue(last_row+1, 2,item);
    setCellValue(last_row+1, 3,spec);
    setCellValue(last_row+1, 4,value);
    setCellValue(last_row+1, 5,res);
    setCellValue(last_row+1, 6,start);
    setCellValue(last_row+1, 7,end);
    saveExcel(fileNametmp);
    freeExcel();
    return;
}

void MainWindow::doRtuQuery(int& regis)
{
    modbus_t *mb;
    uint16_t tab_reg[32]={0};
    QByteArray  aByteArray = ui->virtual_lineEdit->text().toAscii();//先把转化为QByteArray的结果存储起来
    const char *com =  aByteArray.data();//再调用data()或constData()
    qDebug()<<"$COM:"<<com;
    mb = modbus_new_rtu(com, 9600, 'N', 8, 1);   //相同的端口只能同时打开一个
    int addr=ui->addr_lineEdit->text().toInt();
    qDebug()<<"$ADDR:"<<addr;
    modbus_set_slave(mb, addr);  //设置modbus从机地址
    modbus_connect(mb);
    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=ui->resp_to_lineEdit->text().toLong()*1000;   //设置modbus超时时间为1000毫秒
    modbus_set_response_timeout(mb, &t);
    //    int regs=modbus_read_registers(mb, 0, 20, tab_reg);
    //    qDebug()<<"regs:"<<regs;
    int regs;
    QTime t1;
    t1.start();
    while(1)
    {
        QTime t;
        t.start();
        while(t.elapsed()<3000)
            QCoreApplication::processEvents();
        qDebug()<<"wait========";

        int regs=modbus_read_registers(mb, 0, 20, tab_reg);
        qDebug()<<"regs:"<<regs;
        if(regs==20)
        {
            regis=regs;
            break;
        }
        if(t1.elapsed()>=30000)
        {
            regis=regs;
            qDebug()<<"timeout!";
            break;
        }
    }
    for(int i=0;i<20;++i)
    {
        qDebug()<<"reg:"<<i<<":"<<tab_reg[i];
    }
    //    regis=regs;
    modbus_close(mb);
    modbus_free(mb);
}
void MainWindow::InitSocket()
{
    mp_TCPServer = new QTcpServer();
    int port = ui->srv_port_lineEdit->text().toInt();
    QString serverIP=ui->serverIP_lineEdit->text();
    if(!this->mp_TCPServer->isListening())
    {
        //          if(!mp_TCPServer->listen(QHostAddress(serverIP),port))
        if(!mp_TCPServer->listen(QHostAddress::Any,port))
        {
            qDebug()<<"服务器端监听失败！";

        }
        else
        {
            qDebug()<<"服务器监听成功！";
        }
    }
    connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));

}
void MainWindow::SendData(QString sendMsg)
{
    char sendMsgChar[1024] = {0};
    if(sendMsg.isEmpty())
    {
        return;
    }
    strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
    if(!mp_TCPSocket)
        return;

    if(mp_TCPSocket->isValid())
    {
        this->mp_TCPSocket->waitForBytesWritten();
        int sendRe = mp_TCPSocket->write(sendMsgChar, strlen(sendMsgChar));
        if( -1 == sendRe)
        {
            qDebug()<<"服务端发送数据失败！";
        }
    }
    else
    {
        qDebug()<<"套接字无效！";
    }
}
void MainWindow::ServerReadData()
{
    qDebug()<<"ServerReadData() is called";
    char buffer[1024] = {0};
    mp_TCPSocket->read(buffer, 1024);
    if( strlen(buffer) > 0)
    {
        this->rece=buffer;
        qDebug()<<"@received:"<<rece;
        if(rece.indexOf("PM25#EVT2-")!=-1&&rece.indexOf("null,null*")!=-1)
        {
            qDebug()<<tr("收到hermes发送的数据");
            //            mp_TCPSocket->blockSignals(true);
            rece="";
            this->isHermesSend=true;
            this->isHermesSendCompleted=true;

        }

        this->mp_TCPSocket->waitForBytesWritten(-1);
        SendData("SERVER_SEND_DATA");

        if(rece.indexOf("HERMES_RECEIVE_DATA_OK")!=-1)
        {
            qDebug()<<tr("收到hermes返回的数据");
            rece="";
            this->isHermesReceived=true;
            this->isHermesReceivedCompleted=true;
            mp_TCPSocket->blockSignals(true);
        }

    }else
    {
        qDebug()<<"not data received";
        this->isHermesReceived=false;
        this->isHermesSend=false;
    }
}

void MainWindow::ServerNewConnection()
{
    this->mp_TCPServer->waitForNewConnection();
    mp_TCPSocket = mp_TCPServer->nextPendingConnection();
    if(!mp_TCPSocket)
    {
        qDebug()<<tr("未正确获取客户端连接！");
        ui->textBrowser->append(tr("--->请检查网线是否连接正确，然后重新测试！"));
    }
    else
    {
        this->hasNewConnecting=true;
        qDebug()<<tr("成功接受客户端的连接");
        connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
        connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
    }
}
void MainWindow::sServerDisConnection()
{
    qDebug()<<tr("与客户端的连接断开");
    this->isHermesSendCompleted=false;
    this->isHermesReceivedCompleted=false;
    return;
}
void MainWindow::Test(QString testItem,QString spec,QString buf)
{
    QString val,ret;
    if(testItem=="eth_send_test")
    {
        ui->start_pushButton->setEnabled(false);
        if(!this->isEthSendTested)
        {
            if(this->isHermesSend)
            {
                qDebug()<<"hermes send data ok";
                this->test_result&=true;
                qDebug()<<"###test_result:"<<this->test_result;
                ret="PASS";
            }
            else
            {
                qDebug()<<"hermes send data fail";
                this->test_result&=false;
                qDebug()<<"###test_result:"<<this->test_result;
                ret="FAIL";
            }
            this->testCount++;
            qDebug()<<"$test count:"<<this->testCount;
            this->isEthSendTested=true;
            val=(ret=="PASS"?"ETHERNET SEND TEST PASS":"ETHERNET SEND TEST FAIL");
            this->appTestLogShow(ui->SN_lineEdit->text(),testItem,spec,val,ret,"","");
        }
    }
    if(testItem=="eth_receive_test")
    {
        if(!this->isEthReceiveTested)
        {
            if(isHermesReceived)
            {
                qDebug()<<"server send data ok";
                this->test_result&=true;
                qDebug()<<"###test_result:"<<this->test_result;
                ret="PASS";

            }else
            {
                qDebug()<<"server send data fail";
                this->test_result&=false;
                qDebug()<<"###test_result:"<<this->test_result;
                ret="FAIL";
            }
            this->testCount++;
            qDebug()<<"$test count:"<<this->testCount;
            this->isEthReceiveTested=true;
            val=(ret=="PASS"?"ETHERNET RECEIVE TEST PASS":"ETHERNET RECEIVE TEST FAIL");
            this->appTestLogShow(ui->SN_lineEdit->text(),testItem,spec,val,ret,"","");
        }
    }
    if(testItem=="rs485_addr_write")
    {
        if(!this->isRs485AddrWrited)
        {
            QString send="AT+WRITE_DEV_ADDR#";
            send+=ui->addr_lineEdit->text()+";";
            this->sendTestCommand(send);
            ui->textBrowser->append(temp);
            //            if(buf.indexOf("HERMES_ADDR_WRITE_OK")==-1)
            //            {
            //                this->test_result&=false;
            //                 qDebug()<<"###test_result:"<<this->test_result;
            //                ret="FAIL";
            //            }
            //            else
            //            {
            //                this->test_result&=true;
            //                 qDebug()<<"###test_result:"<<this->test_result;
            //                ret="PASS";
            //            }
            this->test_result&=true;
            ret="PASS";
            this->testCount++;
            qDebug()<<"$test count:"<<this->testCount;
            this->isRs485AddrWrited=true;
            val=(ret=="PASS"?"HERMES ADDR WRITE OK":"HERMES_ADDR WRITE FAIL");
            this->appTestLogShow(ui->SN_lineEdit->text(),testItem,spec,val,ret,"","");
        }
    }
    if(testItem=="rs485_communication_test")
    {
        if(!this->isRs485CommTested)
        {

            int regs;
            doRtuQuery(regs);
            qDebug()<<"regs:"<<regs;
            if(regs<=0)
            {
                this->test_result&=false;
                qDebug()<<"###test_result:"<<this->test_result;
                ret="FAIL";

            }
            else
            {
                this->test_result&=true;
                qDebug()<<"###test_result:"<<this->test_result;
                ret="PASS";
            }
            this->testCount++;
            qDebug()<<"$test count:"<<this->testCount;
            this->isRs485CommTested=true;
            val=(ret=="PASS"?"RS485 COMMUNICATION TEST PASS":"RS485 COMMUNICATION TEST FAIL");
            this->appTestLogShow(ui->SN_lineEdit->text(),testItem,spec,val,ret,"","");
        }
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

void MainWindow::on_actionAbout_triggered()
{
    ui->textBrowser->append(tr("Hermes communication test software"));
    ui->textBrowser->append(tr("latest version：1.0.0.5"));
    ui->textBrowser->append(tr("author：王拴阁"));
    ui->textBrowser->append(tr("upgrade：2018-10-17"));
    ui->textBrowser->append(tr("Copyright 2018-2020 The Qt iDPBG MLB AE  All rights reserved."));


    ui->textBrowser->append(tr("------change history 1.0.0.4 2018-10-17-------"));
    ui->textBrowser->append(tr("1.修复测试过程出现以太网收发测试误判问题；"));
    ui->textBrowser->append(tr("2.修复程式测试过程闪退问题"));
    ui->textBrowser->append(tr("3.增加忘记插网线提示"));
    ui->textBrowser->append(tr("4.修复内存消耗过大问题"));
    ui->textBrowser->append(tr("5.取消测试前提示作业员可以测试的提示，将开始测试判断逻辑集成到开始按钮"));
    ui->textBrowser->append(tr("6.增加SN长度及格式判断逻辑，防止作业员输入错误"));
    ui->textBrowser->append(tr("------change history 1.0.0.5 2018-10-18-------"));
    ui->textBrowser->append(tr("7.修复RS485 MODBUS通信测试误判问题，并增加timeout机制"));
}
