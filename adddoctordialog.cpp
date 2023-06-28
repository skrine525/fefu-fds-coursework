#include "adddoctordialog.h"
#include "ui_adddoctordialog.h"
#include "table1.h"
#include "mainwindow.h"

#include <QMessageBox>

AddDoctorDialog::AddDoctorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDoctorDialog)
{
    ui->setupUi(this);

    // Настройка валидации полей
    ui->lineEditPhone->setValidator(
                new QRegExpValidator(QRegExp("^\\d{1,11}$"), ui->lineEditPhone));
    ui->lineEditFullname->setValidator(
                new QRegExpValidator(QRegExp("^(?:[А-ЯЁа-яё]+ ?){0,2}[А-ЯЁа-яё]+$"), ui->lineEditFullname));
    ui->lineEditSpeciality->setValidator(
                new QRegExpValidator(QRegExp("^[А-ЯЁа-яё]+$"), ui->lineEditSpeciality));
}

AddDoctorDialog::~AddDoctorDialog()
{
    delete ui;
}

void AddDoctorDialog::on_pushButtonCancel_clicked()
{
    this->close();
}


void AddDoctorDialog::on_pushButtonAdd_clicked()
{
    // Проверка наличия данных в lineEdit полях
    if(ui->lineEditFullname->text().count(" ") != 2)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"ФИО пациента\" должно содержать 3 слова.");
        return;
    }
    else if(ui->lineEditSpeciality->text().length() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"Специальность\" не должно быть пустым.");
        return;
    }
    else if(ui->lineEditPhone->text().length() != 11)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"Номер телефона\" должно содержать 11 цифр.");
        return;
    }

    table1::Record newRecord;
    newRecord.fullName = ui->lineEditFullname->text();
    newRecord.speciality = ui->lineEditSpeciality->text();
    newRecord.experience = ui->spinBoxExperience->value();
    newRecord.phoneNumber = ui->lineEditPhone->text().toLongLong();

    MainWindow *mainWindow = qobject_cast<MainWindow*>(this->parent());
    mainWindow->addRecordToDoctors(newRecord);
    this->close();
}

