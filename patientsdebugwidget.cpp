#include "patientsdebugwidget.h"
#include "ui_patientsdebugwidget.h"

PatientsDebugWidget::PatientsDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientsDebugWidget)
{
    ui->setupUi(this);
}

PatientsDebugWidget::~PatientsDebugWidget()
{
    delete ui;
}

void PatientsDebugWidget::setPatients(table2::Patients *patients)
{
    patientsPointer = patients;
}

void PatientsDebugWidget::on_pushButtonShowFullNameTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = patientsPointer->fullNameTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

void PatientsDebugWidget::on_pushButtonShowAddressTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = patientsPointer->addressTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

void PatientsDebugWidget::on_pushButtonShowAgeTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = patientsPointer->ageTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

