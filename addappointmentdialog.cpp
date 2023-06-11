#include "addappointmentdialog.h"
#include "ui_addappointmentdialog.h"
#include "table3.h"
#include "mainwindow.h"

#include <QString>
#include <QtDebug>
#include <QMessageBox>

AddAppointmentDialog::AddAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAppointmentDialog)
{
    ui->setupUi(this);

    // Настройка валидации полей Номер врача и Номер пациента
    ui->lineEditDoctorPhone->setValidator(
                new QRegExpValidator(QRegExp("[0-9]*"), ui->lineEditDoctorPhone));
    ui->lineEditPatientPhone->setValidator(
                new QRegExpValidator(QRegExp("[0-9]*"), ui->lineEditPatientPhone));

    // Добавление элементов времени в ComboBox
    for(int i = 8; i < 18; i++)
    {
        QString hourString = QString::number(i);
        if (i < 10)
            hourString = "0" + hourString;
        ui->comboBoxAppointmentTimeHour->addItem(hourString, QVariant(i));
    }
    ui->comboBoxAppointmentTimeMinute->addItem("00", QVariant(0));
    ui->comboBoxAppointmentTimeMinute->addItem("30", QVariant(30));
}

AddAppointmentDialog::~AddAppointmentDialog()
{
    delete ui;
}

void AddAppointmentDialog::on_pushButtonAppointmentActionAdd_clicked()
{
    // Проверка наличия данных в lineEdit полях
    if(ui->lineEditDoctorPhone->text().length() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Укажите Номер врача!");
        return;
    }
    else if(ui->lineEditPatientPhone->text().length() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Укажите Номер пациета!");
        return;
    }


    table3::Record newRecord;

    newRecord.doctorPhoneNumber = ui->lineEditDoctorPhone->text().toLongLong();
    newRecord.patientPhoneNumber = ui->lineEditPatientPhone->text().toLongLong();
    newRecord.appointmentCost = ui->spinBoxAppointmentCost->value();

    table3::Datetime appointmentDatetime;
    appointmentDatetime.day = ui->spinBoxAppointmentDateDay->value();
    appointmentDatetime.month = ui->spinBoxAppointmentDateMonth->value();
    appointmentDatetime.year = ui->spinBoxAppointmentDateYear->value();
    appointmentDatetime.hour = ui->comboBoxAppointmentTimeHour->currentData().toUInt();
    appointmentDatetime.minute = ui->comboBoxAppointmentTimeMinute->currentData().toUInt();
    newRecord.appointmentDatetime = appointmentDatetime;

    MainWindow *mainWindow = qobject_cast<MainWindow*>(this->parent());
    mainWindow->addRecordToAppointments(newRecord);
    this->close();
}


void AddAppointmentDialog::on_pushButtonAppointmentActionCancel_clicked()
{
    this->close();
}

