#include "page_login.h"
#include "ui_page_login.h"
#include <QApplication>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QIODevice>
#include <string.h>
#include <ctime>


QTcpSocket *mSocket = new QTcpSocket();
int begin;
QString name = "";
QString connectTime;
page_login::page_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_login)
{
    ui->setupUi(this);



    connect(mSocket,&QTcpSocket::connected,
            [=]()
            {
                begin = clock();
                time_t timep;
                time(&timep);

                struct tm *p;
                p = gmtime(&timep);


                connectTime.append(QString::number(1900+p->tm_year));
                connectTime.append("-");

                connectTime.append(QString::number(1+p->tm_mon));
                connectTime.append("-");
                connectTime.append(QString::number(p->tm_mday));
                connectTime.append("  ");
                connectTime.append(QString::number(8+p->tm_hour));
                connectTime.append(":");
                connectTime.append(QString::number(p->tm_min));
                connectTime.append(":");
                connectTime.append(QString::number(p->tm_sec));

                qDebug("%s", connectTime.toStdString().data());

                ui->textTestConnect->insertPlainText(tr(connectTime.toStdString().data()));// append current data and display
                ui->textTestConnect->insertPlainText(tr("\n"));
                ui->textTestConnect->moveCursor(QTextCursor::Start);


                ui->textTestConnect->insertPlainText("Connect Success!");
            }
            );

    connect(mSocket,&QTcpSocket::disconnected,
            [=]()
            {
                ui->textTestConnect->setText("Fail to connect！");
            }
            );



}

page_login::~page_login()
{
    delete ui;
}

void page_login::on_LoginButton_clicked()
{
    //Obtain the user name and password
    name = ui->lineEdituser->text();
    QString psd = ui->lineEditpassword->text();
    qDebug("name: %s",name.toUtf8().data());
    qDebug("password: %s",psd.toStdString().data());


    qDebug("name2: %s",name.toLatin1().data());
    //search database user and password
    //send socket to server


    //fail
    // send socket to server
    mSocket->flush();
    mSocket->write(name.toLatin1().data(),name.toLatin1().length());
    mSocket->flush();


    //receive the priority
    connect(mSocket,&QTcpSocket::readyRead,
            [=]()
            {

                QByteArray readprio = mSocket->readAll();
                qDebug("%d", readprio.toInt());

                if(readprio.toInt() == 100){//if the return socket say this user is student 1
                    emit sendStudentSuccess();
                    disconnect(this, SIGNAL(sendStudentSuccess()), 0, 0);
                    mSocket->flush();
                    this->close();

                }else if (readprio.toInt() == 200){//if this user is administrator 2
                    emit sendAdminSuccess();
                    disconnect(this, SIGNAL(sendAdminSuccess()), 0, 0);
                    mSocket->flush();
                    this->close();
                }else{
                    //there is no this user
                    ui->textTestConnect->setText("Connect Success!\nLog in fail: no user!");
                }
            }
            );
    // close this login page





//    connect(mSocket, &QTcpSocket::readyRead,
//             [=]()
//             {
//                //get content from client
//                QByteArray array = mSocket->readAll();
//                // SHOW IN READONLY TEST FIELD
//                //ui->textEditRead->append(array);
//                receivePriority();
//             }
//             );

}

void page_login::on_Exit_Button_clicked()
{
    exit(0);
}

void page_login::on_ConnectButton_clicked()
{
    mSocket->abort();
    QString ip = ui->lineEditIp->text();

//    QString ip = "172.30.139.187";
    qint16 port = 5019;

    mSocket->connectToHost(QHostAddress(ip),port);
    mSocket->waitForConnected(3000);



}

void page_login::client_connect()
{
    qDebug("Connected");
}

void page_login::client_disconnect()
{
    qDebug("Disonnected");
}
