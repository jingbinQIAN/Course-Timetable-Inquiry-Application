#include "admin.h"
#include "ui_admin.h"
#include <QApplication>
#include <mainwindow.h>
#include <string.h>
#include <QIODevice>
#include "page_login.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QDebug>


extern int begin;
extern QString name;
extern QString connectTime;
extern QTcpSocket* mSocket;
Admin::Admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin)
{

    ui->setupUi(this);

    connect(mSocket,&QTcpSocket::connected,
            [=]()
            {
                //ui->textTestConnect->setText("成功连接服务器！");
                qDebug("成功连接服务器！");
            }
            );

}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_SearchStudentButton_clicked()
{
    // initialize the page: clear the textBrowser:FindStudentShow
    ui->FindStudentShow->setText("");

    // search this student name in server(database)
    // show student info in FindStudentShow

    //Obtain the studentsname
    QString stuname = ui->lineEdit->text();
    qDebug("name: %s",stuname.toStdString().data());

    // concatinate info to socket
    QString act5 = "5";
    act5.append(";").append(stuname.toStdString().data());
    qDebug("acti: %s",act5.toStdString().data());

    // here i tried to use Qstring, it works but still have the rewrite socket bug.
    // Ziao FENG:2022/12/0.13
    // i also tried to reinitialize szBuff but still socket prob
    // It can be sure the socket can not clear itself at each send and receive.


    //search database students
    //send socket to server
    mSocket->flush();
    mSocket->write(act5.toStdString().data());



    qDebug("Send socket: %s", act5.toStdString().data());

    //receive the student infomation
    //receive the row number (the number of students)
    connect(mSocket,&QTcpSocket::readyRead,
            [=]()
            {
                QList<QByteArray> buffer;
                while (mSocket->waitForReadyRead(1000)){
                    buffer.append(mSocket->readAll());
                }


                QList<QByteArray>::iterator iter;
                for(iter=buffer.begin();iter!=buffer.end();iter++){
                    ui->FindStudentShow->insertPlainText(tr(iter->toStdString().data()));// append current data and display
                    ui->FindStudentShow->insertPlainText(tr("\n"));
                    ui->FindStudentShow->moveCursor(QTextCursor::Start);// move the cursor to start position
                    qDebug("buffer: %s", iter->toStdString().data());
                    }
                });

}






void Admin::on_AddStudentButton_clicked()
{
    ui->FindStudentShow->setText("");
    // add this student in server(database)
    //Obtain the studentsname
    QString stuname = ui->lineEdit->text();
    qDebug("name: %s",stuname.toStdString().data());

    // here i tried to use Qstring, it works but still have the rewrite socket bug.
    // Ziao FENG:2022/12/0.13
    // i also tried to reinitialize szBuff but still socket prob
    // It can be sure the socket can not clear itself at each send and receive.
    QString acti = "1";
    acti.append(";").append(stuname.toStdString().data());
    qDebug("acti: %s",acti.toStdString().data());
    //search database students

    //send socket to server 
    mSocket->flush();
    mSocket->write(acti.toStdString().data(),acti.toStdString().length());
    mSocket->flush();


    qDebug("Send socket: %s", acti.toStdString().data());


    //receive the student infomation
    //if the return socket say this user is student 1
    //emit sendStudentSuccess();
    //if this user is administrator 2
    connect(mSocket,&QTcpSocket::readyRead,
            [=]()
            {
                if(mSocket->bytesAvailable()<0){
                    qDebug("noread: byte");
                }

                QList<QByteArray> buffer;
                while (mSocket->waitForReadyRead(1000)){
                    qDebug("Already in readyread, append to buffer");
                    buffer.append(mSocket->readAll());
                }
                // show all in buffer
                QList<QByteArray>::iterator iter;
                int count = 0;
                for(iter=buffer.begin();iter!=buffer.end();iter++){
                    if (count==1){
                        ui->FindStudentShow->insertPlainText(tr(iter->toStdString().data()));// append current data and display
                        ui->FindStudentShow->insertPlainText(tr("\n"));
                        ui->FindStudentShow->moveCursor(QTextCursor::Start);// move the cursor to start position
                        qDebug("count: %d",  count);
                    }
                    count++;

                }


//                QByteArray readinfo = mSocket->readAll();
//                qDebug("add student:%s", readinfo.toStdString().data());
//                ui->FindStudentShow->setText(readinfo.toStdString().data());
            }
            );




    // here we search this student again to test if this add success
    //


    // initialize the page: clear the textBrowser:FindStudentShow
   // ui->FindStudentShow->setText("");

    // search this student name in server(database)
    // show student info in FindStudentShow

    //Obtain the studentsname

    // concatinate info to socket


}

void Admin::on_UpdateStudentButton_clicked()
{
    // update this student to administrator

    ui->FindStudentShow->setText("");
    //Obtain the studentsname
    QString stuname = ui->lineEdit->text();
    qDebug("name: %s",stuname.toStdString().data());

    // here i tried to use Qstring, it works but still have the rewrite socket bug.
    // Ziao FENG:2022/12/0.13
    // i also tried to reinitialize szBuff but still socket prob
    // It can be sure the socket can not clear itself at each send and receive.
    QString acti = "2";
    acti.append(";").append(stuname.toStdString().data()).append(";2");
    qDebug("acti: %s",acti.toStdString().data());
    //search database students

    //send socket to server
    mSocket->flush();
    mSocket->write(acti.toStdString().data(),acti.toStdString().length());



    qDebug("Send socket: %s", acti.toStdString().data());

    connect(mSocket,&QTcpSocket::readyRead,
            [=]()
            {
                if(mSocket->bytesAvailable()<0){
                    qDebug("noread: byte");
                }

                QList<QByteArray> buffer;
                while (mSocket->waitForReadyRead(1000)){
                    qDebug("Already in readyread, append to buffer");
                    buffer.append(mSocket->readAll());
                }
                // show all in buffer
                QList<QByteArray>::iterator iter;
                int count = 0;
                for(iter=buffer.begin();iter!=buffer.end();iter++){
                    if (count==1){
                        ui->FindStudentShow->insertPlainText(tr(iter->toStdString().data()));// append current data and display
                        ui->FindStudentShow->insertPlainText(tr("\n"));
                        ui->FindStudentShow->moveCursor(QTextCursor::Start);// move the cursor to start position
                        qDebug("count: %d",  count);
                    }
                    count++;

                }
         });


}

void Admin::on_DeleteStudentButton_clicked()
{
    // delete this student


    ui->FindStudentShow->setText("");
    //Obtain the studentsname
    QString stuname = ui->lineEdit->text();
    qDebug("name: %s",stuname.toStdString().data());

    // here i tried to use Qstring, it works but still have the rewrite socket bug.
    // Ziao FENG:2022/12/0.13
    // i also tried to reinitialize szBuff but still socket prob
    // It can be sure the socket can not clear itself at each send and receive.
    QString acti = "3";
    acti.append(";").append(stuname.toStdString().data());
    qDebug("acti: %s",acti.toStdString().data());
    //search database students

    //send socket to server
    mSocket->flush();
    mSocket->write(acti.toStdString().data(),acti.toStdString().length());



    qDebug("Send socket: %s", acti.toStdString().data());

    connect(mSocket,&QTcpSocket::readyRead,
            [=]()
            {
                if(mSocket->bytesAvailable()<0){
                    qDebug("noread: byte");
                }

                QList<QByteArray> buffer;
                while (mSocket->waitForReadyRead(1000)){
                    qDebug("Already in readyread, append to buffer");
                    buffer.append(mSocket->readAll());
                }
                // show all in buffer
                QList<QByteArray>::iterator iter;
                int count = 0;
                for(iter=buffer.begin();iter!=buffer.end();iter++){
                    if (count==1){
                        ui->FindStudentShow->insertPlainText(tr(iter->toStdString().data()));// append current data and display
                        ui->FindStudentShow->insertPlainText(tr("\n"));
                        ui->FindStudentShow->moveCursor(QTextCursor::Start);// move the cursor to start position
                        qDebug("count: %d",  count);
                    }
                    count++;

                }
         });


}

void Admin::on_ExitButton_clicked()
{
    int end = clock();
    int runtime = 10*(end-begin)/(double)CLOCKS_PER_SEC;
    QString sendtime = QString::number(runtime);
    QString act9 = "9;";
    act9.append(name.toStdString().data()).append(";").append(connectTime.toStdString().data()).append(";").append(sendtime.toStdString().data());
    qDebug("act9: %s",act9.toStdString().data());
    //send to server
    mSocket->flush();
    mSocket->write(act9.toStdString().data());


    qDebug("Send socket: %s", act9.toStdString().data());
    mSocket->flush();

    exit(0);
}
