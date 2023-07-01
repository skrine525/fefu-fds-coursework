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
    ui->textBrowserTree->clear();
    QString html = patientsPointer->fullNameTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}


void PatientsTreeDebugWidget::on_pushButtonShowAddressTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = patientsPointer->addressTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}


void PatientsTreeDebugWidget::on_pushButtonShowAgeTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = patientsPointer->ageTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

