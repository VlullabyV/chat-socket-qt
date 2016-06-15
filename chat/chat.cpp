#include "chat.h"
#include "ui_chat.h"

#define SERVERIP "127.0.0.1"
//#define SERVERIP "172.20.10.4"
#define SERVERPORT 8888
#define MULTIPORT 9999

Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat) {
    ui->setupUi(this);

    ui->listWidget->addItems(ipaddrList);
    ui->listWidget2->addItems(multiaddrList);

    connect(ui->pushButton1, SIGNAL(clicked()), this, SLOT(Start()));
    connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(End()));
    connect(ui->pushButton3, SIGNAL(clicked()), this, SLOT(Send()));
    connect(ui->pushButton4, SIGNAL(clicked()), this, SLOT(Clear()));
    connect(ui->pushButton5, SIGNAL(clicked()), this, SLOT(Join()));
    connect(ui->pushButton6, SIGNAL(clicked()), this, SLOT(Exit()));

    flag = false;

    multist.initbind(MULTIPORT);
    multithread = new MyThread();
    multithread->setSocket(&multist);
    multithread->start();
    connect(multithread, SIGNAL(update(QString)), this, SLOT(update(QString)));
}

Chat::~Chat() {
    if (flag) {
        st.exit();
        st.end();
        flag = false;
        thread->stop();
        multithread->stop();
    }
    delete ui;
}

//“开始聊天”按钮，连接服务器
void Chat::Start() {
    if (flag) {
        QMessageBox::information(this,tr("通知"),tr("已经开始聊天！"), QMessageBox::Yes);
        return;
    }
    char ipaddr[20] = SERVERIP;
    if (!st.init(ipaddr, SERVERPORT)) {
        QMessageBox::information(this,tr("失败"),tr("网络连接初始化失败！"), QMessageBox::Yes);
    }
    if (!st.join()) {
        QMessageBox::information(this,tr("失败"),tr("连接服务器失败！"), QMessageBox::Yes);
    }
    flag = true;

    thread = new MyThread();
    thread->setSocket(&st);
    thread->start();

    connect(thread, SIGNAL(update(QString)), this, SLOT(update(QString)));
}

//“断开连接”按钮，断开与服务器的连接
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
    thread->stop();
    ui->listWidget->clear();
}

//“发送”按钮，发送消息
void Chat::Send() {
    char ipaddr[20];
    strcpy(ipaddr, ui->textEdit1->toPlainText().toLocal8Bit().data());
    char port[10];
    strcpy(port, ui->textEdit2->toPlainText().toLocal8Bit().data());
    char multiaddr[20];
    strcpy(multiaddr, ui->textEdit3->toPlainText().toLocal8Bit().data());
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
    if (0 == strcmp(ipaddr, "") && 0 == strcmp(multiaddr, "")) {
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
        if (-1 == ipaddrList.indexOf(addr)) {
            QMessageBox::information(this,tr("失败"),tr("对方未在线，请向在线列表中的地址发送消息！"), QMessageBox::Yes);
            return;
        }
        if(!st.send(ipaddr, port, msg)) {
            QMessageBox::information(this,tr("失败"),tr("发送消息失败！"), QMessageBox::Yes);
        }
        ui->textEdit4->clear();
        QString sendMsg("我：");
        sendMsg.append(msg);
        ui->textBrowser->append(sendMsg);
    } else {
        QString addr(multiaddr);
//        if (-1 == multiaddrList.indexOf(addr)) {
//            QMessageBox::information(this,tr("失败"),tr("未加入该群组，请先加入该群组！"), QMessageBox::Yes);
//            return;
//        }
        sprintf(port, "%d", MULTIPORT);
        if(!st.send(multiaddr, port, msg)) {
            QMessageBox::information(this,tr("失败"),tr("发送消息失败！"), QMessageBox::Yes);
        }
        ui->textEdit4->clear();
        QString sendMsg("我：");
        sendMsg.append(msg);
        ui->textBrowser->append(sendMsg);
    }
}

//"清屏”按钮，清空“聊天消息”和“输入消息”窗口
void Chat::Clear() {
    ui->textBrowser->clear();
    ui->textEdit4->clear();
}

//接收其他其他线程的消息，接收消息并更新“聊天消息”窗口
void Chat::update(QString msg) {
    if (msg.indexOf("iplist:") == 0) {
        msg = msg.right(msg.size()-7);
        ipaddrList = msg.split(" ");
        ui->listWidget->clear();
        ui->listWidget->addItems(ipaddrList);
    } else {
        ui->textBrowser->append(msg);
    }
}


//重载“退出”按钮，退出程序，断开与服务器的连接
void Chat::closeEvent(QCloseEvent *event)
{
    int res = QMessageBox::question(this, tr("退出"), tr("是否退出程序?"), QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
        if (res == QMessageBox::Yes) {
            if (flag) {
                st.exit();
                st.end();
                flag = false;
                thread->stop();
            }
            event->accept();
        } else
            event->ignore();
}

//“加入群组”按钮，加入指定多播地址群组
void Chat::Join() {
    char multiaddr[20];
    strcpy(multiaddr, ui->textEdit5->toPlainText().toLocal8Bit().data());
    if (0 == strcmp(multiaddr, "")) {
        QMessageBox::information(this,tr("失败"),tr("未填写群组地址！"), QMessageBox::Yes);
        return;
    }
    multist.joinGroup(multiaddr);
    QString addr(multiaddr);
    if (-1 == multiaddrList.indexOf(addr)) {
        multiaddrList.append(addr);
    } else {
        QMessageBox::information(this,tr("失败"),tr("已经加入该群组！"), QMessageBox::Yes);
        return;
    }
    ui->listWidget2->clear();
    ui->listWidget2->addItems(multiaddrList);
    ui->textEdit5->clear();
}

//“退出群组”按钮，退出指定多播地址群组
void Chat::Exit() {
    char multiaddr[20];
    strcpy(multiaddr, ui->textEdit5->toPlainText().toLocal8Bit().data());
    if (0 == strcmp(multiaddr, "")) {
        QMessageBox::information(this,tr("失败"),tr("未填写群组地址！"), QMessageBox::Yes);
        return;
    }
    multist.exitGroup(multiaddr);
    QString addr(multiaddr);
    int pos = multiaddrList.indexOf(addr);
    QStringList::iterator it;
    if (pos >= 0) {
        it = multiaddrList.begin() + pos;
        multiaddrList.erase(it);
    } else {
        QMessageBox::information(this,tr("失败"),tr("未加入该群组！"), QMessageBox::Yes);
        return;
    }
    ui->listWidget2->clear();
    ui->listWidget2->addItems(multiaddrList);
    ui->textEdit5->clear();
}
