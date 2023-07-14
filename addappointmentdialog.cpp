#include "addappointmentdialog.h"
#include "ui_addappointmentdialog.h"
#include "table3.h"

#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>

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
        QMessageBox::warning(this, "Внимание", "Поле \"Номер Врача\" должно содержать 11 цифр.");
        return;
    }
    else if(ui->lineEditPatientPhone->text().length() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"Номер Пациента\" должно содержать 11 цифр.");
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

    auto result = mainWindow->insertRecordToAppointments(newRecord);
    if(result == MainWindow::InsertionResult::Exists)
        QMessageBox::warning(this, "Внимание", "У Врача уже существует запись на указанную Дату и Время.");
    else if(result == MainWindow::InsertionResult::DoctorFailure)
        QMessageBox::warning(this, "Внимание", "Врача с указанным номером телефона не существует.");
    else if(result == MainWindow::InsertionResult::PatientFailure)
        QMessageBox::warning(this, "Внимание", "Пациента с указанным номером телефона не существует.");
    else
    {
        mainWindow->getStatusBar()->showMessage("Записи - Запись добавлена.");
        this->close();
    }
}


void AddAppointmentDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void AddAppointmentDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}
