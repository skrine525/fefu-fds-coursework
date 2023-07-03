#include "searchpatientdialog.h"
#include "ui_searchpatientdialog.h"

#include <QMessageBox>

SearchPatientDialog::SearchPatientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchPatientDialog)
{
    ui->setupUi(this);

    // Настройка валидации полей
    ui->lineEditPhone->setValidator(
                new QRegExpValidator(QRegExp("^\\d{1,11}$"), ui->lineEditPhone));
    ui->lineEditFullname->setValidator(
                new QRegExpValidator(QRegExp("^(?:[А-ЯЁа-яё]+ ?){0,2}[А-ЯЁа-яё]+$"), ui->lineEditFullname));
    ui->lineEditRegion->setValidator(
                new QRegExpValidator(QRegExp("^[А-ЯЁа-яё]+$"), ui->lineEditRegion));

    // Добавление элементов в Район ComboBox
    QStringList districts;
    districts.append("Ленинский");
    districts.append("Первомайский");
    districts.append("Первореченский");
    districts.append("Советский");
    districts.append("Фрунзенский");
    for(int i = 0; i < districts.count(); i++)
        ui->comboBoxDistrict->addItem(districts[i], QVariant(districts[i]));
}

SearchPatientDialog::~SearchPatientDialog()
{
    delete ui;
}

void SearchPatientDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}

void SearchPatientDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void SearchPatientDialog::on_radioButtonFullname_toggled(bool checked)
{
    ui->lineEditFullname->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 0;
    }
}

void SearchPatientDialog::on_radioButtonAddress_toggled(bool checked)
{
    ui->labelRegion->setEnabled(checked);
    ui->labelDistrict->setEnabled(checked);
    ui->lineEditRegion->setEnabled(checked);
    ui->comboBoxDistrict->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 1;
    }
}

void SearchPatientDialog::on_radioButtonAge_toggled(bool checked)
{
    ui->spinBoxAge->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 2;
    }
}

void SearchPatientDialog::on_radioButtonPhone_toggled(bool checked)
{
    ui->lineEditPhone->setEnabled(checked);
    if(checked)
    {
        ui->pushButtonSearch->setEnabled(true);
        fieldIndex = 3;
    }
}

void SearchPatientDialog::on_pushButtonSearch_clicked()
{
    table2::Record searchRecord;

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
        if(ui->lineEditRegion->text().length() != 0)
        {
            searchRecord.region = ui->lineEditRegion->text();
            searchRecord.district = ui->comboBoxDistrict->currentData().toString();
        }
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"Регион\" не должно быть пустым.");
            return;
        }
    }
    else if(fieldIndex == 2)
    {
        searchRecord.age = ui->spinBoxAge->value();
    }
    else if(fieldIndex == 3)
    {
        if(ui->lineEditPhone->text().length() == 11)
            searchRecord.phoneNumber = ui->lineEditPhone->text().toLongLong();
        else
        {
            QMessageBox::warning(this, "Внимание", "Поле \"Номер телефона\" должно содержать 11 цифр.");
            return;
        }
    }

    // Отображаем результат поиск и закрываем диалоговое окно
    mainWindow->showPatientSearchResult(searchRecord, fieldIndex);
    this->close();
}

