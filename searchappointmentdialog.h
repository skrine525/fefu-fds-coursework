#ifndef SEARCHAPPOINTMENTDIALOG_H
#define SEARCHAPPOINTMENTDIALOG_H

#include <QDialog>

namespace Ui {
class SearchAppointmentDialog;
}

class SearchAppointmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchAppointmentDialog(QWidget *parent = nullptr);
    ~SearchAppointmentDialog();

private:
    Ui::SearchAppointmentDialog *ui;
};

#endif // SEARCHAPPOINTMENTDIALOG_H
