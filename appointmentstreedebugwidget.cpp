#include "appointmentstreedebugwidget.h"
#include "ui_appointmentstreedebugwidget.h"

AppointmentsTreeDebugWidget::AppointmentsTreeDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppointmentsTreeDebugWidget)
{
    ui->setupUi(this);
}

AppointmentsTreeDebugWidget::~AppointmentsTreeDebugWidget()
{
    delete ui;
}

void AppointmentsTreeDebugWidget::setAppointments(table3::Appointments &appointments)
{
    appointmentsPointer = &appointments;
}


void AppointmentsTreeDebugWidget::on_pushButtonShowDoctorPhoneNumberTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->doctorPhoneNumberTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}


void AppointmentsTreeDebugWidget::on_pushButtonShowPatientPhoneNumberTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->patientPhoneNumberTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}


void AppointmentsTreeDebugWidget::on_pushButtonShowAppointmentDatetimeTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->appointmentDatetimeTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}


void AppointmentsTreeDebugWidget::on_pushButtonShowAppointmentCostTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->appointmentCostTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

