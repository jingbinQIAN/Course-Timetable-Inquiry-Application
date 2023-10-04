#include "choiceadmin.h"
#include "ui_choiceadmin.h"
#include <QApplication>

ChoiceAdmin::ChoiceAdmin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoiceAdmin)
{
    ui->setupUi(this);
    //if administrator choose the student_info button turn to admin page
    auto h = [&](){
        m_dlgadmin.show();
    };
    connect(this, &ChoiceAdmin::sendAdminSuccess,this,h);
    // if administrator chiise the timetable_info button turn to time table page
    auto i = [&](){
        m_time.show();
    };
    connect(this, &ChoiceAdmin::sendTimetableSuccess,this,i);
}

ChoiceAdmin::~ChoiceAdmin()
{
    delete ui;
}

void ChoiceAdmin::on_StudentButton_clicked()
{
    // change to admin page, send signal
    emit sendAdminSuccess();
}


void ChoiceAdmin::on_AdminButton_clicked()
{
    // change to timetable page
    emit sendTimetableSuccess();

}
