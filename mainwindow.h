#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "table1.h"
#include "table3.h"
#include "table2.h"
#include "doctorsdebugwidget.h"
#include "patientsdebugwidget.h"
#include "appointmentsdebugwidget.h"

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

    bool addRecordToDoctors(table1::Record record);
    bool addRecordToPatients(table2::Record record);
    int addRecordToAppointments(table3::Record record);

    bool removeRecordFromDoctors(int index);
    bool removeRecordFromPatients(int index);
    bool removeRecordFromAppointments(int index);

    void showDoctorSearchResult(table1::Record record, int fieldIndex);
    void showPatientSearchResult(table2::Record record, int fieldIndex);
    void showAppointmentSearchResult(table3::Record record, int fieldIndex);

private slots:
    void on_menuFileOpen_triggered();

    void on_pushButtonAppointmentsAdd_clicked();

    void on_menuFileSave_triggered();

    void on_pushButtonAppointmentsSearch_clicked();

    void on_pushButtonAppointmentsClearSearch_clicked();

    void on_menuFileCreate_triggered();

    void on_pushButtonAppointmentsDelete_clicked();

    void on_pushButtonPatientsAdd_clicked();

    void on_pushButtonDoctorsAdd_clicked();

    void on_menuDebugDoctors_triggered();

    void on_menuDebugPatients_triggered();

    void on_menuFileExit_triggered();

    void on_menuDebugAppointments_triggered();

    void on_pushButtonDoctorsSearch_clicked();

    void on_pushButtonDoctorsClearSearch_clicked();

    void on_pushButtonPatientsSeatch_clicked();

    void on_pushButtonPatientsClearSearch_clicked();

    void on_pushButtonDoctorsDelete_clicked();

private:
    Ui::MainWindow *ui;
    table1::Doctors doctors;
    table2::Patients patients;
    table3::Appointments appointments;

    DoctorsDebugWidget doctorsDebugWidget;
    PatientsDebugWidget patientsDebugWidget;
    AppointmentsDebugWidget appointmentsDebugWidget;

    void resetViewAndData();
};
#endif // MAINWINDOW_H
