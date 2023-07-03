#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"
#include "table2.h"
#include "mainwindow.h"

#include <QMessageBox>

AddPatientDialog::AddPatientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPatientDialog)
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

AddPatientDialog::~AddPatientDialog()
{
    delete ui;
}

void AddPatientDialog::on_pushButtonCancel_clicked()
{
    this->close();
}


void AddPatientDialog::on_pushButtonAdd_clicked()
{
    // Проверка наличия данных в lineEdit полях
    if(ui->lineEditFullname->text().count(" ") != 2)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"ФИО пациента\" должно содержать 3 слова.");
        return;
    }
    else if(ui->lineEditRegion->text().length() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"Регион\" не должно быть пустым.");
        return;
    }
    else if(ui->lineEditPhone->text().length() != 11)
    {
        QMessageBox::warning(this, "Внимание", "Поле \"Номер телефона\" должно содержать 11 цифр.");
        return;
    }


    table2::Record newRecord;
    newRecord.fullName = ui->lineEditFullname->text();
    newRecord.region = ui->lineEditRegion->text();
    newRecord.phoneNumber = ui->lineEditPhone->text().toLongLong();
    newRecord.district = ui->comboBoxDistrict->currentData().toString();
    newRecord.age = ui->spinBoxAge->value();

    if(mainWindow->addRecordToPatients(newRecord))
        this->close();
}

void AddPatientDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}
