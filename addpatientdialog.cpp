#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"
#include "table2.h"

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

}

