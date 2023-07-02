#include "searchdoctordialog.h"
#include "ui_searchdoctordialog.h"

SearchDoctorDialog::SearchDoctorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDoctorDialog)
{
    ui->setupUi(this);
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
        fieldIndex = 2;
    }
}

void SearchDoctorDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}

void SearchDoctorDialog::on_pushButtonSearch_clicked()
{

}

