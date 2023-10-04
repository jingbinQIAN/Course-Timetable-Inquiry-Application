#include "timetable.h"
#include "ui_timetable.h"
#include "page_login.h"

#include <QApplication>
#include <string.h>
#include <QIODevice>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QDebug>
extern QTcpSocket *mSocket;
extern int begin;
extern QString name;
extern QString connectTime;
timetable::timetable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::timetable)
{
    ui->setupUi(this);
}

timetable::~timetable()
{
    delete ui;
}

void timetable::on_ConfirmButton_clicked()
{
    // update timetable from database

    // initialize textbrowser to empty
    ui->textDisplay->setText("");

    // get inputs
    // obtain mid input
    QString mid = ui->midInput->text();
    qDebug("obtain input mid: %s",mid.toStdString().data());
    // obtain room input
    QString room = ui->roomInput->text();
    qDebug("obtain input room_info: %s",room.toStdString().data());
    // obtain time input
    QString time = ui->timeInput->text();
    qDebug("obtain input time_info: %s",time.toStdString().data());
    //concatinate info to socket
    QString acti = "4";
    acti.append(";").append(mid.toStdString().data()).append(";").append(room.toStdString().data()).append(";").append(time.toStdString().data());
    qDebug("acti: %s",acti.toStdString().data());

    //send socket to server
    mSocket->flush();
    mSocket->write(acti.toStdString().data());


    qDebug("Send socket: %s", acti.toStdString().data());
    // receive from server
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
                        ui->textDisplay->insertPlainText(tr(iter->toStdString().data()));// append current data and display
                        ui->textDisplay->insertPlainText(tr("\n"));
                        ui->textDisplay->moveCursor(QTextCursor::Start);// move the cursor to start position
                        qDebug("count: %d",  count);
                    }
                    count++;
                }
         });
}

void timetable::on_ExitButton_clicked()
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
