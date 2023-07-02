#ifndef ADDDOCTORDIALOG_H
#define ADDDOCTORDIALOG_H

#include "mainwindow.h"

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
    void setMainWindow(MainWindow *mainWindow);

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();

private:
    Ui::AddDoctorDialog *ui;
    MainWindow *mainWindow;
};

#endif // ADDDOCTORDIALOG_H
