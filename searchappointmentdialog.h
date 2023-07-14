#ifndef SEARCHAPPOINTMENTDIALOG_H
#define SEARCHAPPOINTMENTDIALOG_H

#include "mainwindow.h"

#include <QDialog>
#include <QRegExpValidator>

namespace Ui {
class SearchAppointmentDialog;
}

class SearchAppointmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchAppointmentDialog(QWidget *parent = nullptr);
    ~SearchAppointmentDialog();
    void setMainWindow(MainWindow *mainWindow);

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonSearch_clicked();

    void on_radioButtonDoctorPhone_toggled(bool checked);

    void on_radioButtonPatientPhone_toggled(bool checked);

    void on_radioButtonDatetime_toggled(bool checked);

    void on_radioButtonCost_toggled(bool checked);

    void on_radioButtonCompositeField_toggled(bool checked);

private:
    Ui::SearchAppointmentDialog *ui;
    MainWindow *mainWindow;
    unsigned fieldIndex;
};

#endif // SEARCHAPPOINTMENTDIALOG_H
