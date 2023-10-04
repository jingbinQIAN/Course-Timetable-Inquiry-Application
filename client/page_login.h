#ifndef PAGE_LOGIN_H
#define PAGE_LOGIN_H

#include <QWidget>

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>

namespace Ui {
class page_login;
}

class page_login : public QWidget
{
    Q_OBJECT

public:
    explicit page_login(QWidget *parent = nullptr);
    ~page_login();
    //QTcpSocket *mSocket;

private slots:

    void client_connect();
    void client_disconnect();

    void on_LoginButton_clicked();

    void on_Exit_Button_clicked();

    void on_ConnectButton_clicked();


signals:
    void sendStudentSuccess();
    void sendAdminSuccess();

private:
    Ui::page_login *ui;
//    QTcpSocket *mSocket;
};

#endif // PAGE_LOGIN_H

