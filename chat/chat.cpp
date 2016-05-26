#include "chat.h"
#include "ui_chat.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 8888

Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat) {
    ui->setupUi(this);

    ui->listWidget->addItems(list);

    connect(ui->pushButton1, SIGNAL(clicked()), this, SLOT(Start()));
    connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(End()));
    connect(ui->pushButton3, SIGNAL(clicked()), this, SLOT(Send()));
    connect(ui->pushButton4, SIGNAL(clicked()), this, SLOT(Clear()));

    flag = false;
}

Chat::~Chat() {
    if (flag) {
        st.exit();
        st.end();
        flag = false;
        thread.stop();
    }
    delete ui;
}

void Chat::Start() {
    if (flag) {
        QMessageBox::information(this,tr("通知"),tr("已经开始聊天！"), QMessageBox::Yes);
        return;
    }
    char ip[20] = SERVERIP;
    st.setInfo(ip, SERVERPORT);
    if (!st.init()) {
        QMessageBox::information(this,tr("失败"),tr("网络连接初始化失败！"), QMessageBox::Yes);
    }
    if (!st.join()) {
        QMessageBox::information(this,tr("失败"),tr("连接服务器失败！"), QMessageBox::Yes);
    }
    flag = true;

    thread.setSocket(&st);
    thread.start();
    connect(&thread, SIGNAL(update(QString)), this, SLOT(update(QString)));
}

void Chat::End() {
    if (!flag) {
        QMessageBox::information(this,tr("通知"),tr("已经断开连接！"), QMessageBox::Yes);
        return;
    }
    if (st.exit())
        QMessageBox::information(this,tr("成功"),tr("断开连接成功！"), QMessageBox::Yes);
    else
        QMessageBox::information(this,tr("失败"),tr("断开连接失败！"), QMessageBox::Yes);
    st.end();
    flag = false;
    thread.stop();
    ui->listWidget->clear();
}

void Chat::Send() {
    char ipaddr[20];
    strcpy(ipaddr, ui->textEdit1->toPlainText().toLocal8Bit().data());
    char port[10];
    strcpy(port, ui->textEdit2->toPlainText().toLocal8Bit().data());
    char mutiaddr[20];
    strcpy(mutiaddr, ui->textEdit3->toPlainText().toLocal8Bit().data());
    char msg[SIZE];
    strcpy(msg, ui->textEdit4->toPlainText().toLocal8Bit().data());

    if (!flag) {
        QMessageBox::information(this,tr("失败"),tr("未连接服务器，请点击“开始聊天”！"), QMessageBox::Yes);
        return;
    }

    if (0 == strcmp(msg, "")) {
        QMessageBox::information(this,tr("失败"),tr("未填写发送消息内容！"), QMessageBox::Yes);
        return;
    }
    if (0 == strcmp(ipaddr, "") && 0 == strcmp(mutiaddr, "")) {
        QMessageBox::information(this,tr("失败"),tr("未填写对方IP地址或组播地址！"), QMessageBox::Yes);
        return;
    }

    if (0 != strcmp(ipaddr, "")) {
        if (0 == strcmp(port, "")) {
            QMessageBox::information(this,tr("失败"),tr("未填写端口号！"), QMessageBox::Yes);
            return;
        }
        QString addr(ipaddr);
        addr.append(":");
        addr.append(port);
        if (-1 == list.indexOf(addr)) {
            QMessageBox::information(this,tr("失败"),tr("对方未在线，请向在线列表中的地址发送消息！"), QMessageBox::Yes);
            return;
        }
    } else {
        if (0 != strcmp(mutiaddr, "224.0.0.1")) {
            QMessageBox::information(this,tr("失败"),tr("未找到对应群组！"), QMessageBox::Yes);
            return;
        }
        strcpy(ipaddr, mutiaddr);
        strcpy(port, "1000");
    }

    if(!st.send(ipaddr, port, msg)) {
        QMessageBox::information(this,tr("失败"),tr("发送消息失败！"), QMessageBox::Yes);
    }
    ui->textEdit4->clear();
    ui->textBrowser->append(msg);
}

void Chat::Clear() {
    ui->textBrowser->clear();
    ui->textEdit4->clear();
}

void Chat::update(QString msg) {
    if (msg.indexOf("iplist:") == 0) {
        msg = msg.right(msg.size()-7);
        list = msg.split(" ");
        ui->listWidget->clear();
        ui->listWidget->addItems(list);
    } else {
        ui->textBrowser->append(msg);
    }
}

void Chat::closeEvent(QCloseEvent *event)
{
    int res = QMessageBox::question(this, tr("退出"), tr("是否退出程序?"), QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
        if (res == QMessageBox::Yes) {
            if (flag) {
                st.exit();
                st.end();
                flag = false;
                thread.stop();
            }
            event->accept();
        } else
            event->ignore();
}
