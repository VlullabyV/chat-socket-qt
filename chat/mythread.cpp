#include "mythread.h"

MyThread::MyThread(QObject *parent) :
    QThread(parent) {
    stopped = false;
}

MyThread::MyThread(Socket *socket, QObject *parent) :
    QThread(parent) {
    st = socket;
    stopped = false;
}

void MyThread::setSocket(Socket *socket) {
    st = socket;
}

void MyThread::run() {
    while(!stopped) {
        char recv_buf[SIZE];
        memset(recv_buf, 0, SIZE);
        st->recv(recv_buf);
        emit update(QString(recv_buf));
        QThread::sleep(1);
    }
}

void MyThread::stop() {
    stopped = true;
}
