#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class AddAppointmentDialog;
}

class AddAppointmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAppointmentDialog(QWidget *parent = nullptr);
    ~AddAppointmentDialog();

private slots:
    void on_pushButtonAppointmentActionAdd_clicked();

    void on_pushButtonAppointmentActionCancel_clicked();

private:
    Ui::AddAppointmentDialog *ui;
    MainWindow *mainWindow;
};

#endif // ADDAPPOINTMENTDIALOG_H
