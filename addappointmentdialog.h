#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include "mainwindow.h"

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
    void setMainWindow(MainWindow *mainWindow);

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::AddAppointmentDialog *ui;
    MainWindow *mainWindow;
};

#endif // ADDAPPOINTMENTDIALOG_H
