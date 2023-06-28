#ifndef ADDDOCTORDIALOG_H
#define ADDDOCTORDIALOG_H

#include <QDialog>

namespace Ui {
class AddDoctorDialog;
}

class AddDoctorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDoctorDialog(QWidget *parent = nullptr);
    ~AddDoctorDialog();

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();

private:
    Ui::AddDoctorDialog *ui;
};

#endif // ADDDOCTORDIALOG_H
