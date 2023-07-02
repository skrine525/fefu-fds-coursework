#ifndef SEARCHDOCTORDIALOG_H
#define SEARCHDOCTORDIALOG_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class SearchDoctorDialog;
}

class SearchDoctorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDoctorDialog(QWidget *parent = nullptr);
    ~SearchDoctorDialog();
    void setMainWindow(MainWindow *mainWindow);

private slots:
    void on_pushButtonCancel_clicked();

    void on_radioButtonFullname_toggled(bool checked);

    void on_radioButtonSpeciality_toggled(bool checked);

    void on_radioButtonExperience_toggled(bool checked);

    void on_radioButtonPhone_toggled(bool checked);

private:
    Ui::SearchDoctorDialog *ui;
    MainWindow *mainWindow;
    unsigned fieldIndex;
};

#endif // SEARCHDOCTORDIALOG_H
