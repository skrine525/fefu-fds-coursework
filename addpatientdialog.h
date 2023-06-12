#ifndef ADDPATIENTDIALOG_H
#define ADDPATIENTDIALOG_H

#include <QDialog>

namespace Ui {
class AddPatientDialog;
}

class AddPatientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPatientDialog(QWidget *parent = nullptr);
    ~AddPatientDialog();

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();

private:
    Ui::AddPatientDialog *ui;
};

#endif // ADDPATIENTDIALOG_H
