#include "addappointmentdialog.h"
#include "ui_addappointmentdialog.h"
#include "table3.h"

#include <QString>
#include <QDebug>
#include <QMessageBox>

AddAppointmentDialog::AddAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAppointmentDialog)
{
    ui->setupUi(this);

    // Настройка валидации полей Номер врача и Номер пациента
    ui->lineEditDoctorPhone->setValidator(
                new QRegExpValidator(QRegExp("\\d{1,11}$"), ui->lineEditDoctorPhone));
    ui->lineEditPatientPhone->setValidator(
                new QRegExpValidator(QRegExp("\\d{1,11}$"), ui->lineEditPatientPhone));

    // Добавление элементов времени в ComboBox
    for(int i = 8; i < 18; i++)
    {
        QString hourString = QString::number(i);
        if (i < 10)
            hourString = "0" + hourString;
        ui->comboBoxTimeHour->addItem(hourString, QVariant(i));
    }
    ui->comboBoxTimeMinute->addItem("00", QVariant(0));
    ui->comboBoxTimeMinute->addItem("30", QVariant(30));
}

AddAppointmentDialog::~AddAppointmentDialog()
{
    delete ui;
}

void AddAppointmentDialog::on_pushButtonAdd_clicked()
{
    // Проверка наличия данных в lineEdit полях
    if(ui->lineEditDoctorPhone->text().length() < 11)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"Номер врача\" должно содержать 11 цифр.");
        return;
    }
    else if(ui->lineEditPatientPhone->text().length() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"Номер пациента\" должно содержать 11 цифр.");
        return;
    }


    table3::Record newRecord;
    newRecord.doctorPhoneNumber = ui->lineEditDoctorPhone->text().toLongLong();
    newRecord.patientPhoneNumber = ui->lineEditPatientPhone->text().toLongLong();
    newRecord.appointmentCost = ui->spinBoxCost->value();
    table3::Datetime appointmentDatetime;
    appointmentDatetime.day = ui->spinBoxDateDay->value();
    appointmentDatetime.month = ui->spinBoxDateMonth->value();
    appointmentDatetime.year = ui->spinBoxDateYear->value();
    appointmentDatetime.hour = ui->comboBoxTimeHour->currentData().toUInt();
    appointmentDatetime.minute = ui->comboBoxTimeMinute->currentData().toUInt();
    newRecord.appointmentDatetime = appointmentDatetime;

    int result = mainWindow->addRecordToAppointments(newRecord);
    if(result == 1)
        QMessageBox::warning(this, "Внимание", "У врача или пациента уже существует запись на это время.");
    else if(result == 2)
        QMessageBox::warning(this, "Внимание", "Доктора с указанным номером телефона не существует.");
    else if(result == 3)
        QMessageBox::warning(this, "Внимание", "Пациента с указанным номером телефона не существует.");
    else
        this->close();
}


void AddAppointmentDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void AddAppointmentDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}
