#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include "mythread.h"
#include "socket.h"

namespace Ui {
class Chat;
}

class Chat : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = 0);
    ~Chat();
    void closeEvent(QCloseEvent *event);

public slots:
    void update(QString msg);

private:
    Ui::Chat *ui;

    QStringList ipaddrList;     //存储在线用户列表
    QStringList multiaddrList;  //存储已加入群组列表

    Socket st;      //用于发送消息和接收单播消息的Socket类
    Socket multist; //用于接收多播消息的Socket类
    MyThread *thread;       //用于接收单播消息的线程类
    MyThread *multithread;  //用于接收多播消息的线程类

    bool flag;      //用于标识是否连接上服务器

private slots:
    void Start();
    void End();
    void Send();
    void Clear();
    void Join();
    void Exit();
};

#endif // CHAT_H
