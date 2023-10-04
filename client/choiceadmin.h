#ifndef CHOICEADMIN_H
#define CHOICEADMIN_H

#include <QWidget>
#include "admin.h"// this is admin page about student info manage
#include "timetable.h"// this is admin page about timetable info manage


namespace Ui {
class ChoiceAdmin;
}

class ChoiceAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit ChoiceAdmin(QWidget *parent = nullptr);
    ~ChoiceAdmin();

private slots:
    void on_StudentButton_clicked();

    void on_AdminButton_clicked();

signals:
    void sendTimetableSuccess();
    void sendAdminSuccess();


private:
    Ui::ChoiceAdmin *ui;
//    timetable m_dlgtimetable;
    Admin m_dlgadmin;
    timetable m_time;
};

#endif // CHOICEADMIN_H
