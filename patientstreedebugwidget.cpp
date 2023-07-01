#include "patientstreedebugwidget.h"
#include "ui_patientstreedebugwidget.h"

PatientsTreeDebugWidget::PatientsTreeDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientsTreeDebugWidget)
{
    ui->setupUi(this);
}

PatientsTreeDebugWidget::~PatientsTreeDebugWidget()
{
    delete ui;
}

void PatientsTreeDebugWidget::setPatients(table2::Patients *patients)
{
    patientsPointer = patients;
}

void PatientsTreeDebugWidget::on_pushButtonShowFullnameTree_clicked()
{

}


void PatientsTreeDebugWidget::on_pushButtonShowAddressTree_clicked()
{

}


void PatientsTreeDebugWidget::on_pushButtonShowAgeTree_clicked()
{

}

