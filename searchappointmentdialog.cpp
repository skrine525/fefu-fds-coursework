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
    ui->lineEditAppointmentDoctorPhone->setValidator(
                new QRegExpValidator(QRegExp("[0-9]*"), ui->lineEditAppointmentDoctorPhone));
    ui->lineEditAppointmentPatientPhone->setValidator(
                new QRegExpValidator(QRegExp("[0-9]*"), ui->lineEditAppointmentPatientPhone));

    // Добавление элементов времени в ComboBox
    for(int i = 8; i < 18; i++)
    {
        QString hourString = QString::number(i);
        if (i < 10)
            hourString = "0" + hourString;
        ui->comboBoxAppointmentDatetimeHour->addItem(hourString, QVariant(i));
    }
    ui->comboBoxAppointmentDatetimeMinute->addItem("00", QVariant(0));
    ui->comboBoxAppointmentDatetimeMinute->addItem("30", QVariant(30));

    // Отключаем все элементы ввода
    ui->pushButtonSearch->setEnabled(false);
    ui->lineEditAppointmentDoctorPhone->setEnabled(false);
    ui->lineEditAppointmentPatientPhone->setEnabled(false);
    ui->spinBoxAppointmentDatetimeDay->setEnabled(false);
    ui->spinBoxAppointmentDatetimeMonth->setEnabled(false);
    ui->spinBoxAppointmentDatetimeYear->setEnabled(false);
    ui->comboBoxAppointmentDatetimeHour->setEnabled(false);
    ui->comboBoxAppointmentDatetimeMinute->setEnabled(false);
    ui->spinBoxAppointmentCost->setEnabled(false);
}

SearchAppointmentDialog::~SearchAppointmentDialog()
{
    delete ui;
}

void SearchAppointmentDialog::on_radioButtonAppointmentDoctorPhone_toggled(bool checked)
{
    ui->lineEditAppointmentDoctorPhone->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 0;
    }
}


void SearchAppointmentDialog::on_radioButtonAppointmentPatientPhone_toggled(bool checked)
{
    ui->lineEditAppointmentPatientPhone->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 1;
    }
}


void SearchAppointmentDialog::on_radioButtonAppointmentDatetime_toggled(bool checked)
{
    ui->spinBoxAppointmentDatetimeDay->setEnabled(checked);
    ui->spinBoxAppointmentDatetimeMonth->setEnabled(checked);
    ui->spinBoxAppointmentDatetimeYear->setEnabled(checked);
    ui->comboBoxAppointmentDatetimeHour->setEnabled(checked);
    ui->comboBoxAppointmentDatetimeMinute->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 2;
    }
}


void SearchAppointmentDialog::on_radioButtonAppointmentCost_toggled(bool checked)
{
    ui->spinBoxAppointmentCost->setEnabled(checked);
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

        if(ui->lineEditAppointmentDoctorPhone->text().length() > 0)
        {
            searchRecord.doctorPhoneNumber = ui->lineEditAppointmentDoctorPhone->text().toLongLong();

            MainWindow *mainWindow = qobject_cast<MainWindow*>(this->parent());
            mainWindow->showAppointmentSearchResult(searchRecord, fieldIndex);
            this->close();
        }
        else
            QMessageBox::warning(this, "Внимание", "Укажите Номер врача!");
    }
    else if(fieldIndex == 1)
    {
        // Номер пациента

        if(ui->lineEditAppointmentPatientPhone->text().length() > 0)
        {
            searchRecord.patientPhoneNumber = ui->lineEditAppointmentPatientPhone->text().toLongLong();

            MainWindow *mainWindow = qobject_cast<MainWindow*>(this->parent());
            mainWindow->showAppointmentSearchResult(searchRecord, fieldIndex);
            this->close();
        }
        else
            QMessageBox::warning(this, "Внимание", "Укажите Номер пациета!");
    }
    else if(fieldIndex == 2)
    {
        // Дата и Время приёма

        searchRecord.appointmentDatetime.day = ui->spinBoxAppointmentDatetimeDay->value();
        searchRecord.appointmentDatetime.month = ui->spinBoxAppointmentDatetimeMonth->value();
        searchRecord.appointmentDatetime.year = ui->spinBoxAppointmentDatetimeYear->value();
        searchRecord.appointmentDatetime.hour = ui->comboBoxAppointmentDatetimeHour->currentData().toUInt();
        searchRecord.appointmentDatetime.minute = ui->comboBoxAppointmentDatetimeMinute->currentData().toUInt();

        MainWindow *mainWindow = qobject_cast<MainWindow*>(this->parent());
        mainWindow->showAppointmentSearchResult(searchRecord, fieldIndex);
        this->close();
    }
    else if(fieldIndex == 3)
    {
        // Стоимость

        searchRecord.appointmentCost = ui->spinBoxAppointmentCost->value();

        MainWindow *mainWindow = qobject_cast<MainWindow*>(this->parent());
        mainWindow->showAppointmentSearchResult(searchRecord, fieldIndex);
        this->close();
    }
}

