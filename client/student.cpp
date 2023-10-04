#include "student.h"
#include "ui_student.h"
#include "page_login.h"

extern QTcpSocket* mSocket;
extern int begin;
extern QString name;
extern QString connectTime;
Student::Student(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Student)
{
    ui->setupUi(this);
}

Student::~Student()
{
    delete ui;
}

void Student::on_StudentSearchButton_clicked()
{
    // search this course title from database(server)
    // get course name from input

    ui->timetableDisplay->setText("");

    QString coursename = ui->lineEdit->text();
    qDebug("name: %s",coursename.toStdString().data());

    // concatinate info to socket
    QString act0 = "0";
    act0.append(";").append(coursename.toStdString().data());
    qDebug("acti: %s",act0.toStdString().data());

    //send to server
    mSocket->flush();
    mSocket->write(act0.toStdString().data());


    qDebug("Send socket: %s", act0.toStdString().data());
    mSocket->flush();


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
                for(iter=buffer.begin();iter!=buffer.end();iter++){
                    ui->timetableDisplay->insertPlainText(tr(iter->toStdString().data()));// append current data and display
                    ui->timetableDisplay->insertPlainText(tr("\n"));
                    ui->timetableDisplay->moveCursor(QTextCursor::Start);// move the cursor to start position
                    qDebug("buffer: %s", iter->toStdString().data());
                }
                mSocket->flush();

//                QByteArray readinfo = mSocket->readAll();
//                qDebug("add student:%s", readinfo.toStdString().data());
//                ui->FindStudentShow->setText(readinfo.toStdString().data());
            }
            );

}

void Student::on_ButtonExit_clicked()
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
