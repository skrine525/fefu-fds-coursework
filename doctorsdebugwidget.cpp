#include "doctorsdebugwidget.h"
#include "ui_doctorsdebugwidget.h"

DoctorsDebugWidget::DoctorsDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorsDebugWidget)
{
    ui->setupUi(this);
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

