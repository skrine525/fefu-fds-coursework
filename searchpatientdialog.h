#ifndef SEARCHPATIENTDIALOG_H
#define SEARCHPATIENTDIALOG_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class SearchPatientDialog;
}

class SearchPatientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchPatientDialog(QWidget *parent = nullptr);
    ~SearchPatientDialog();
    void setMainWindow(MainWindow *mainWindow);

private slots:
    void on_pushButtonCancel_clicked();

    void on_radioButtonFullname_toggled(bool checked);

    void on_radioButtonAddress_toggled(bool checked);

    void on_radioButtonAge_toggled(bool checked);

    void on_radioButtonPhone_toggled(bool checked);

    void on_pushButtonSearch_clicked();

private:
    Ui::SearchPatientDialog *ui;
    MainWindow *mainWindow;
    unsigned fieldIndex;
};

#endif // SEARCHPATIENTDIALOG_H
