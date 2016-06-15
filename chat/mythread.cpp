#include "mythread.h"

MyThread::MyThread(QObject *parent) :
    QThread(parent) {
    stopped = false;
}

//设置指定的Socket类实例
void MyThread::setSocket(Socket *socket) {
    st = socket;
}

//线程运行
void MyThread::run() {
    while(!stopped) {
        char recv_buf[SIZE];
        memset(recv_buf, 0, SIZE);
        if (st->recv(recv_buf))
            emit update(QString(recv_buf));
        QThread::sleep(1);
    }
}

//线程结束
void MyThread::stop() {
    stopped = true;
}
