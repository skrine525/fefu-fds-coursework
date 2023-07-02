#include "searchdoctordialog.h"
#include "ui_searchdoctordialog.h"

#include <QMessageBox>

SearchDoctorDialog::SearchDoctorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDoctorDialog)
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

SearchDoctorDialog::~SearchDoctorDialog()
{
    delete ui;
}

void SearchDoctorDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void SearchDoctorDialog::on_radioButtonFullname_toggled(bool checked)
{
    ui->lineEditFullname->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 0;
    }
}

void SearchDoctorDialog::on_radioButtonSpeciality_toggled(bool checked)
{
    ui->lineEditSpeciality->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 1;
    }
}

void SearchDoctorDialog::on_radioButtonExperience_toggled(bool checked)
{
    ui->spinBoxExperience->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 2;
    }
}

void SearchDoctorDialog::on_radioButtonPhone_toggled(bool checked)
{
    ui->lineEditPhone->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 3;
    }
}

void SearchDoctorDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}

void SearchDoctorDialog::on_pushButtonSearch_clicked()
{
    table1::Record searchRecord;

    if(fieldIndex == 0)
    {
        // ФИО

        if(ui->lineEditFullname->text().count(" ") == 2)
            searchRecord.fullName = ui->lineEditFullname->text();
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"ФИО пациента\" должно содержать 3 слова.");
            return;
        }
    }
    else if(fieldIndex == 1)
    {
        // Специальность

        if(ui->lineEditSpeciality->text().length() > 0)
            searchRecord.speciality = ui->lineEditSpeciality->text();
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"Специальность\" не должно быть пустым.");
            return;
        }
    }
    else if(fieldIndex == 2)
    {
        // Стаж

        searchRecord.experience = ui->spinBoxExperience->value();
    }
    else if(fieldIndex == 3)
    {
        // Номер телефона

        if(ui->lineEditPhone->text().length() == 11)
            searchRecord.phoneNumber = ui->lineEditPhone->text().toULongLong();
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"Номер телефона\" должно содержать 11 цифр.");
            return;
        }
    }
    else
        return;

    // Отображаем результат поиск и закрываем диалоговое окно
    mainWindow->showDoctorSearchResult(searchRecord, fieldIndex);
    this->close();
}

