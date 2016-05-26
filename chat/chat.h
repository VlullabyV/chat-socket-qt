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

    QStringList list;

    Socket st;
    MyThread thread;
    bool flag;

private slots:
    void Start();
    void End();
    void Send();
    void Clear();
};

#endif // CHAT_H
