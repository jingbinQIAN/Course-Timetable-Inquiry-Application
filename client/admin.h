#ifndef ADMIN_H
#define ADMIN_H



#include "page_login.h"
#include "student.h"

#include <QWidget>
#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
namespace Ui {
class Admin;
}
class ChoiceAdmin;//这里自己定义了
class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

private slots:
    void on_SearchStudentButton_clicked();

    void on_AddStudentButton_clicked();

    void on_UpdateStudentButton_clicked();

    void on_DeleteStudentButton_clicked();

    void on_ExitButton_clicked();

private:
    Ui::Admin *ui;
    //ChoiceAdmin *m_dlgChoice;//这里加了星号
    // 见csdn
    // AB class 那个解决方法https://blog.csdn.net/yamanda/article/details/85175801?utm_medium=distribute.pc_feed_404.none-task-blog-2~default~BlogCommendFromBaidu~Rate-1-85175801-blog-null.pc_404_mixedpudn&depth_1-utm_source=distribute.pc_feed_404.none-task-blog-2~default~BlogCommendFromBaidu~Rate-1-85175801-blog-null.pc_404_mixedpud


//    Admin m_dlgAdmin;
};

#endif // ADMIN_H
