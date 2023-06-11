#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "table3.h"
#include "appointmentstreedebugwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addRecordToAppointments(table3::Record record);

private slots:
    void on_menuFileFileExit_triggered();

    void on_menuFileOpen_triggered();

    void on_pushButtonAppointmentsAdd_clicked();

    void on_menuDebugAppointmentsShowTree_triggered();

    void on_menuFileSave_triggered();

    void on_pushButtonAppointmentsSearch_clicked();

private:
    Ui::MainWindow *ui;
    AppointmentsTreeDebugWidget appointmentsTreeDebugWidget;
    table3::Appointments appointments;
};
#endif // MAINWINDOW_H
