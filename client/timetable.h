#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QWidget>

namespace Ui {
class timetable;
}

class timetable : public QWidget
{
    Q_OBJECT

public:
    explicit timetable(QWidget *parent = nullptr);
    ~timetable();

private slots:
    void on_ConfirmButton_clicked();

    void on_ExitButton_clicked();

private:
    Ui::timetable *ui;
};

#endif // TIMETABLE_H
