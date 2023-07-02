#ifndef ADDPATIENTDIALOG_H
#define ADDPATIENTDIALOG_H

#include "mainwindow.h"

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
    void setMainWindow(MainWindow *mainWindow);

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();

private:
    Ui::AddPatientDialog *ui;
    MainWindow *mainWindow;
};

#endif // ADDPATIENTDIALOG_H
