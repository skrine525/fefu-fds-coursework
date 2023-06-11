#include "searchappointmentdialog.h"
#include "ui_searchappointmentdialog.h"

SearchAppointmentDialog::SearchAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchAppointmentDialog)
{
    ui->setupUi(this);

    // Добавление элементов времени в ComboBox
    for(int i = 8; i < 18; i++)
    {
        QString hourString = QString::number(i);
        if (i < 10)
            hourString = "0" + hourString;
        ui->comboBoxAppointmentDatetimeHour->addItem(hourString);
    }
    ui->comboBoxAppointmentDatetimeMinute->addItem("00");
    ui->comboBoxAppointmentDatetimeMinute->addItem("30");

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
