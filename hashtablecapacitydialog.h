#ifndef HASHTABLECAPACITYDIALOG_H
#define HASHTABLECAPACITYDIALOG_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class HashTableCapacityDialog;
}

class HashTableCapacityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HashTableCapacityDialog(QWidget *parent = nullptr);
    ~HashTableCapacityDialog();

    void setMainWindow(MainWindow *mainWindow);
    bool getResult();

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonApply_clicked();

private:
    Ui::HashTableCapacityDialog *ui;
    MainWindow *mainWindow;
    bool result;
};

#endif // HASHTABLECAPACITYDIALOG_H
