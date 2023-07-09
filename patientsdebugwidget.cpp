#include "patientsdebugwidget.h"
#include "ui_patientsdebugwidget.h"

PatientsDebugWidget::PatientsDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientsDebugWidget)
{
    ui->setupUi(this);

    // Инициализация таблицы
    ui->tableWidgetHashTable->setColumnCount(6);
    QStringList doctorLabels;
    doctorLabels.append("Index");
    doctorLabels.append("Hash 1");
    doctorLabels.append("Hash 2");
    doctorLabels.append("Key");
    doctorLabels.append("Value");
    doctorLabels.append("Status");
    ui->tableWidgetHashTable->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetHashTable->setHorizontalHeaderLabels(doctorLabels);
    ui->tableWidgetHashTable->resizeColumnsToContents();
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


void PatientsDebugWidget::on_pushButtonShowHashTable_clicked()
{
    ui->tableWidgetHashTable->clearContents();
    ui->tableWidgetHashTable->setRowCount(0);
    patientsPointer->phoneNumberHashTable->printToQTableWidget(ui->tableWidgetHashTable);
    ui->tableWidgetHashTable->resizeColumnsToContents();
    ui->tableWidgetHashTable->horizontalHeader()->setStretchLastSection(true);
}

