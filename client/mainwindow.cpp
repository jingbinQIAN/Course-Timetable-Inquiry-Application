#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIODevice>

//#include "HostInfoFetch.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    m_dlgLogin.show();
    //this->hide();

//    auto f = [&](){
//        this->show();
//    };
//    connect(&m_dlgLogin, &page_login::sendLoginSuccess,this,f);

//    type = ;//get user type

    // one stu

    auto g = [&](){
       m_dlgStudent.show();
    };
    connect(&m_dlgLogin, &page_login::sendStudentSuccess,this,g);


    // one admin

    auto h = [&](){
       m_dlgChoice.show();
    };
    connect(&m_dlgLogin, &page_login::sendAdminSuccess,this,h);

}

MainWindow::~MainWindow()
{

    delete ui;
}

