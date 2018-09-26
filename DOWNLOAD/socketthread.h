

#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H
#include<QThread>

class socketThread : public QThread
{
     Q_OBJECT
public:
    socketThread();
    ~socketThread();
};

#endif // SOCKETTHREAD_H
