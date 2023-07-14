#include "searchappointmentdialog.h"
#include "ui_searchappointmentdialog.h"
#include "table3.h"
#include "mainwindow.h"

#include <QMessageBox>

SearchAppointmentDialog::SearchAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchAppointmentDialog)
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

SearchAppointmentDialog::~SearchAppointmentDialog()
{
    delete ui;
}

void SearchAppointmentDialog::on_radioButtonDoctorPhone_toggled(bool checked)
{
    ui->lineEditDoctorPhone->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 0;
    }
}

void SearchAppointmentDialog::on_radioButtonPatientPhone_toggled(bool checked)
{
    ui->lineEditPatientPhone->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 1;
    }
}

void SearchAppointmentDialog::on_radioButtonDatetime_toggled(bool checked)
{
    ui->spinBoxDateDay->setEnabled(checked);
    ui->spinBoxDateMonth->setEnabled(checked);
    ui->spinBoxDateYear->setEnabled(checked);
    ui->comboBoxTimeHour->setEnabled(checked);
    ui->comboBoxTimeMinute->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 2;
    }
}

void SearchAppointmentDialog::on_radioButtonCost_toggled(bool checked)
{
    ui->spinBoxCost->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 3;
    }
}

void SearchAppointmentDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void SearchAppointmentDialog::on_pushButtonSearch_clicked()
{
    table3::Record searchRecord;

    if(fieldIndex == 0)
    {
        // Номер врача

        if(ui->lineEditDoctorPhone->text().length() == 11)
            searchRecord.doctorPhoneNumber = ui->lineEditDoctorPhone->text().toLongLong();
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"Номер врача\" должно содержать 11 цифр.");
            return;
        }
    }
    else if(fieldIndex == 1)
    {
        // Номер пациента

        if(ui->lineEditPatientPhone->text().length() == 11)
            searchRecord.patientPhoneNumber = ui->lineEditPatientPhone->text().toLongLong();
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"Номер пациента\" должно содержать 11 цифр.");
            return;
        }
    }
    else if(fieldIndex == 2)
    {
        // Дата и Время приёма

        searchRecord.appointmentDatetime.day = ui->spinBoxDateDay->value();
        searchRecord.appointmentDatetime.month = ui->spinBoxDateMonth->value();
        searchRecord.appointmentDatetime.year = ui->spinBoxDateYear->value();
        searchRecord.appointmentDatetime.hour = ui->comboBoxTimeHour->currentData().toUInt();
        searchRecord.appointmentDatetime.minute = ui->comboBoxTimeMinute->currentData().toUInt();
    }
    else if(fieldIndex == 3)
        // Стоимость
        searchRecord.appointmentCost = ui->spinBoxCost->value();
    else if(fieldIndex == 4)
    {
        // Составное поле

        if(ui->lineEditDoctorPhone->text().length() == 11)
            searchRecord.doctorPhoneNumber = ui->lineEditDoctorPhone->text().toLongLong();
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"Номер врача\" должно содержать 11 цифр.");
            return;
        }

        searchRecord.appointmentDatetime.day = ui->spinBoxDateDay->value();
        searchRecord.appointmentDatetime.month = ui->spinBoxDateMonth->value();
        searchRecord.appointmentDatetime.year = ui->spinBoxDateYear->value();
        searchRecord.appointmentDatetime.hour = ui->comboBoxTimeHour->currentData().toUInt();
        searchRecord.appointmentDatetime.minute = ui->comboBoxTimeMinute->currentData().toUInt();
    }
    else
        return;

    // Отображаем результат поиск и закрываем диалоговое окно
    mainWindow->showAppointmentSearchResult(searchRecord, fieldIndex);
    this->close();
}

void SearchAppointmentDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}

void SearchAppointmentDialog::on_radioButtonCompositeField_toggled(bool checked)
{
    ui->spinBoxDateDay->setEnabled(checked);
    ui->spinBoxDateMonth->setEnabled(checked);
    ui->spinBoxDateYear->setEnabled(checked);
    ui->comboBoxTimeHour->setEnabled(checked);
    ui->comboBoxTimeMinute->setEnabled(checked);
    ui->lineEditDoctorPhone->setEnabled(checked);
    ui->labelCompositeField->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 4;
    }
}

