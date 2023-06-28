#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "table1.h"
#include "table3.h"
#include "table2.h"
#include "appointmentstreedebugwidget.h"
#include "patientstreedebugwidget.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addRecordToDoctors(table1::Record record);
    void addRecordToAppointments(table3::Record record);
    void addRecordToPatients(table2::Record record);

    void showAppointmentSearchResult(table3::Record record, int fieldIndex);

private slots:
    void on_menuFileFileExit_triggered();

    void on_menuFileOpen_triggered();

    void on_pushButtonAppointmentsAdd_clicked();

    void on_menuDebugAppointmentsShowTree_triggered();

    void on_menuFileSave_triggered();

    void on_pushButtonAppointmentsSearch_clicked();

    void on_pushButtonAppointmentsClearSearch_clicked();

    void on_menuFileCreate_triggered();

    void on_pushButtonAppointmentsDelete_clicked();

    void on_pushButtonPatientsAdd_clicked();

    void on_pushButtonDoctorsAdd_clicked();

    void on_menuDebugPatientsShowTree_triggered();

private:
    Ui::MainWindow *ui;
    table1::Doctor doctors;
    table2::Patients patients;
    table3::Appointments appointments;

    AppointmentsTreeDebugWidget* appointmentsTreeDebugWidget;
    PatientsTreeDebugWidget* patientsTreeDebugWidget;

    void resetViewAndData();
};
#endif // MAINWINDOW_H
