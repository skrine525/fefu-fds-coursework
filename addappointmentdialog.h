#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>

namespace Ui {
class AddAppointmentDialog;
}

class AddAppointmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAppointmentDialog(QWidget *parent = nullptr);
    ~AddAppointmentDialog();

private:
    Ui::AddAppointmentDialog *ui;
};

#endif // ADDAPPOINTMENTDIALOG_H
