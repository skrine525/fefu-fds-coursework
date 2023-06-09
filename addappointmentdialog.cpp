#include "addappointmentdialog.h"
#include "ui_addappointmentdialog.h"
#include "table3.h"
#include "mainwindow.h"
#include <QString>
#include <QtDebug>

AddAppointmentDialog::AddAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAppointmentDialog)
{
    ui->setupUi(this);
}

AddAppointmentDialog::~AddAppointmentDialog()
{
    delete ui;
}

void AddAppointmentDialog::on_pushButtonAppointmentActionAdd_clicked()
{
    table3::Record newRecord;

    newRecord.doctorPhoneNumber = ui->lineEditDoctorPhone->text().toLongLong();
    newRecord.patientPhoneNumber = ui->lineEditPatientPhone->text().toLongLong();
    newRecord.appointmentCost = ui->spinBoxAppointmentCost->value();

    table3::Datetime appointmentDatetime;
    appointmentDatetime.day = ui->spinBoxAppointmentDateDay->value();
    appointmentDatetime.month = ui->spinBoxAppointmentDateMonth->value();
    appointmentDatetime.year = ui->spinBoxAppointmentDateYear->value();
    appointmentDatetime.hour = ui->spinBoxAppointmentTimeHour->value();
    appointmentDatetime.minute = ui->spinBoxAppointmentTimeMinute->value();
    newRecord.appointmentDatetime = appointmentDatetime;

    MainWindow *mainWindow = qobject_cast<MainWindow*>(this->parent());
    mainWindow->addRecordToAppointments(newRecord);
    this->close();
}


void AddAppointmentDialog::on_pushButtonAppointmentActionCancel_clicked()
{
    this->close();
}

