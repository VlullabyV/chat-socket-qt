#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "socket.h"

class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(QObject *parent = 0);
    explicit MyThread(Socket *socket, QObject *parent = 0);
    void setSocket(Socket *socket);
    void run();
    void stop();

signals:
    void update(QString msg);

private:
    Socket *st;
    bool stopped;

};

#endif // MYTHREAD_H
