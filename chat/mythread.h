#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "socket.h"

class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(QObject *parent = 0);
    void setSocket(Socket *socket);
    void run();
    void stop();

signals:
    void update(QString msg);

private:
    Socket *st;     //接收该实例中socket的消息
    bool stopped;   //线程是否停止标识

};

#endif // MYTHREAD_H
