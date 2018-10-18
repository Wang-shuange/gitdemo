#include "mythread.h"
#include<qt_windows.h>
#include<QDebug>
#include<QDir>
#include<QMutex>
extern QString cmd;
extern QMutex mutex;
extern QString log;
extern HANDLE hP,hT;
MyThread::MyThread(QObject *parent) : QObject(parent)
{

}

MyThread::~MyThread()
{

}
TCHAR *MyThread::CharToWchar(const QString &str)
{
    //    count=1;
    QByteArray ba = str.toUtf8();
    char *data = ba.data(); //以上两步不能直接简化为“char *data = str.toUtf8().data();”
    int charLen = strlen(data);
    int len = MultiByteToWideChar(CP_ACP, 0, data, charLen, NULL, 0);
    TCHAR *buf = new TCHAR[len + 1];
    MultiByteToWideChar(CP_ACP, 0, data, charLen, buf, len);
    buf[len] = '\0';
    return buf;
}
void MyThread::download_infoSlot(QString cmd)
{
   qDebug()<<"received from master thread:" <<cmd;
}

void MyThread::download_Save()
{
    emit downloadStartNotify("DOWNLOAD IS READY!");
    LPCWSTR outputFile = TEXT("download_info.txt");
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hFile = CreateFile(
                (LPCWSTR)outputFile,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                &sa,
                OPEN_ALWAYS | TRUNCATE_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
    si.wShowWindow = SW_HIDE; //不显示创建的进程的窗口
    si.hStdOutput = hFile;//重定向输出句柄为之前创建的文件句柄
    qDebug()<<"@@@@@@cmd:"<<cmd;
    TCHAR *cmdline=CharToWchar(cmd);
    if (CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
    {
        hT=pi.hThread;
        hP=pi.hProcess;
        WaitForSingleObject(pi.hProcess, INFINITE);//等待打开的进程结束并关闭相关句柄
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);

    }
    else
    {
        int err = GetLastError();
        qDebug()<<"err:"<<err;
    }
//    this->kill_P(hP);
    emit downloadFinishNotify();
    logHandle();
}
void MyThread::kill_P(HANDLE hPro)
{
    DWORD dwEC = 0;
    BOOL b = GetExitCodeProcess(
    hPro,     // handle to the process

    &dwEC              // termination status
    );
    if (b)
    {
    TerminateProcess( hPro, dwEC );
    }
}

//void MyThread::download_Save()
//{
//    emit downloadStartNotify("DOWNLOAD IS READY!");
//    LPCWSTR outputFile = TEXT("download_info.txt");
//    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
//    HANDLE hFile = CreateFile(
//                (LPCWSTR)outputFile,
//                GENERIC_READ | GENERIC_WRITE,
//                FILE_SHARE_READ | FILE_SHARE_WRITE,
//                &sa,
//                OPEN_ALWAYS /*| TRUNCATE_EXISTING*/,
//                FILE_ATTRIBUTE_NORMAL,
//                NULL);

//    if (hFile == INVALID_HANDLE_VALUE)
//    {
//        return;
//    }
//    STARTUPINFO si = { sizeof(si) };
//    PROCESS_INFORMATION pi = { 0 };
//    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
//    si.wShowWindow = SW_HIDE; //不显示创建的进程的窗口
//    si.hStdOutput = hFile;//重定向输出句柄为之前创建的文件句柄
//    QString send="STMFlashLoader.exe -c --pn ";
//    QString s="46";
//    send+=s;
//    send+=" --br ";
//    send+="115200";
//    send+=" --db ";
//    send+="8";
//    send+=" --pr EVEN ";
//    send+=" --sb 1 --ec OFF";
//    send+="  --to 10000 ";
//    send+="   -Dtr --Hi -Rts --Lo  -i STM32_High-density_256K ";
//    send+=" -e --all -d --a @08000000 --fn c:\\APP.hex ";
//    qDebug()<<"@@@@@@cmd:"<<send;
//    TCHAR *cmdline=CharToWchar(send);
//    if (CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
//    {
//        WaitForSingleObject(pi.hProcess, INFINITE);//等待打开的进程结束并关闭相关句柄
//        ::CloseHandle(pi.hThread);
//        ::CloseHandle(pi.hProcess);
//    }
//    else
//    {
//        int err = GetLastError();
//        qDebug()<<"err:"<<err;
//    }
//    emit downloadFinishNotify();
//    logHandle();
//}
void MyThread::logHandle()
{
    mutex.lock();
    QString fileNametmp =  QDir::currentPath()+"/download_info.txt";
    QDir::toNativeSeparators(fileNametmp);
    QFile file(fileNametmp);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<file.errorString();
        return;
    }
    QTextStream in(&file);
    QString myText = in.readAll();
    log=myText;
    if(myText.indexOf("[KO]")!=-1||myText.indexOf("This version is not intended to support")!=-1)
    {
       emit logReadNotify("[KO]");
       this->kill_P(hP);

    }else
        emit logReadNotify("[OK]");

    qDebug() << myText;
    file.close();
    mutex.unlock();
}
//void MyThread::download_Save()
//{
//    emit downloadStartNotify("DOWNLOAD IS READY!");
//    LPCWSTR outputFile = TEXT("download_info.txt");
//    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
//    HANDLE hFile = CreateFile(
//                (LPCWSTR)outputFile,
//                GENERIC_READ | GENERIC_WRITE,
//                FILE_SHARE_READ | FILE_SHARE_WRITE,
//                &sa,
//                OPEN_ALWAYS /*| TRUNCATE_EXISTING*/,
//                FILE_ATTRIBUTE_NORMAL,
//                NULL);

//    if (hFile == INVALID_HANDLE_VALUE)
//    {
//        return;
//    }
//    STARTUPINFO si = { sizeof(si) };
//    PROCESS_INFORMATION pi = { 0 };
//    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
//    si.wShowWindow = SW_HIDE; //不显示创建的进程的窗口
//    si.hStdOutput = hFile;//重定向输出句柄为之前创建的文件句柄
//    QString send="STMFlashLoader.exe -c --pn ";
//    QString s="46";
//    send+=s;
//    send+=" --br ";
//    send+="115200";
//    send+=" --db ";
//    send+="8";
//    send+=" --pr EVEN ";
//    send+=" --sb 1 --ec OFF";
//    send+="  --to 10000 ";
//    send+="   -Dtr --Hi -Rts --Lo  -i STM32_High-density_256K ";
//    send+=" -e --all -d --a @08000000 --fn c:\\APP.hex ";

//    qDebug()<<"@@@@@@cmd:"<<send;
//    TCHAR *cmdline=CharToWchar(send);
//    if (CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
//    {
//        WaitForSingleObject(pi.hProcess, INFINITE);//等待打开的进程结束并关闭相关句柄
//        ::CloseHandle(pi.hThread);
//        ::CloseHandle(pi.hProcess);
//    }
//    else
//    {
//        int err = GetLastError();
//        qDebug()<<"err:"<<err;
//    }
//    emit downloadFinishNotify();
//    int siz= GetFileSize(hFile,NULL);
//    qDebug()<<"file size:"<<siz;
//    CloseHandle(hFile);    //关闭文件句柄
//    QString fileNametmp =  QDir::currentPath()+"/download_info.txt";
//    QDir::toNativeSeparators(fileNametmp);
//    QFile file(fileNametmp);
//    if(!file.open(QFile::ReadOnly | QFile::Text))
//    {
//        qDebug()<<file.errorString();
//        return;
//    }
//    QTextStream in(&file);
//    QString myText = in.readAll();
//    if(myText.indexOf("[KO]")==-1)
//    {

//    }
//    qDebug() << myText;
//    file.close();
//}
