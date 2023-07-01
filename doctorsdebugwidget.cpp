#include "doctorsdebugwidget.h"
#include "ui_doctorsdebugwidget.h"

DoctorsDebugWidget::DoctorsDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorsDebugWidget)
{
    ui->setupUi(this);

    // Инициализация таблицы Доктора
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

DoctorsDebugWidget::~DoctorsDebugWidget()
{
    delete ui;
}

void DoctorsDebugWidget::setDoctors(table1::Doctors *doctors)
{
    doctorsPointer = doctors;
}

void DoctorsDebugWidget::on_pushButtonShowFullNameTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = doctorsPointer->fullNameTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

void DoctorsDebugWidget::on_pushButtonShowSpecialityTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = doctorsPointer->specialityTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

void DoctorsDebugWidget::on_pushButtonShowExperienceTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = doctorsPointer->experienceTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}


void DoctorsDebugWidget::on_pushButtonShowHashTable_clicked()
{
    ui->tableWidgetHashTable->clearContents();
    ui->tableWidgetHashTable->setRowCount(0);
    doctorsPointer->phoneNumberHashTable.printToQTableWidget(ui->tableWidgetHashTable);
    ui->tableWidgetHashTable->resizeColumnsToContents();
    ui->tableWidgetHashTable->horizontalHeader()->setStretchLastSection(true);
}

