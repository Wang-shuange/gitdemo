#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include<windows.h>
class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    ~MyThread();

    TCHAR *CharToWchar(const QString &str);
    void logHandle();
    void kill_P(HANDLE hPro);
signals:
    void downloadStartNotify(QString s);
    void logReadNotify(QString s);
    void downloadFinishNotify();
public slots:
    void download_Save();
    void download_infoSlot(QString cmd);
};

#endif // MYTHREAD_H
