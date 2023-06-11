#ifndef SEARCHAPPOINTMENTDIALOG_H
#define SEARCHAPPOINTMENTDIALOG_H

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

private slots:
    void on_radioButtonAppointmentDoctorPhone_toggled(bool checked);

    void on_radioButtonAppointmentPatientPhone_toggled(bool checked);

    void on_radioButtonAppointmentDatetime_toggled(bool checked);

    void on_radioButtonAppointmentCost_toggled(bool checked);

    void on_pushButtonCancel_clicked();

    void on_pushButtonSearch_clicked();

private:
    Ui::SearchAppointmentDialog *ui;
    QRegExpValidator* validatorDoctorPhone;
    QRegExpValidator* validatorPatientPhone;
    unsigned fieldIndex;
};

#endif // SEARCHAPPOINTMENTDIALOG_H
