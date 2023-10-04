#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "page_login.h"
#include "student.h"
#include "choiceadmin.h"
#include <QObject>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    page_login m_dlgLogin;
    Student m_dlgStudent;


    ChoiceAdmin m_dlgChoice;

};
#endif // MAINWINDOW_H
