#ifndef STUDENT_H
#define STUDENT_H

#include <QWidget>

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include "page_login.h"

namespace Ui {
class Student;
}

class Student : public QWidget
{
    Q_OBJECT

public:
    explicit Student(QWidget *parent = nullptr);
    ~Student();

private slots:
    void on_StudentSearchButton_clicked();

    void on_ButtonExit_clicked();

private:
    Ui::Student *ui;
};

#endif // STUDENT_H
