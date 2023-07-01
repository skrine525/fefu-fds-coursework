#include "appointmentsdebugwidget.h"
#include "ui_appointmentsdebugwidget.h"

AppointmentsDebugWidget::AppointmentsDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppointmentsDebugWidget)
{
    ui->setupUi(this);
}

AppointmentsDebugWidget::~AppointmentsDebugWidget()
{
    delete ui;
}

void AppointmentsDebugWidget::setAppointments(table3::Appointments *appointments)
{
    appointmentsPointer = appointments;
}

void AppointmentsDebugWidget::on_pushButtonShowDoctorPhoneNumberTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->doctorPhoneNumberTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

void AppointmentsDebugWidget::on_pushButtonShowPatientPhoneNumberTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->patientPhoneNumberTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

void AppointmentsDebugWidget::on_pushButtonShowAppointmentDatetimeTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->appointmentDatetimeTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

void AppointmentsDebugWidget::on_pushButtonShowAppointmentCostTree_clicked()
{
    ui->textBrowserTree->clear();
    QString html = appointmentsPointer->appointmentCostTree.getPrintableHtml(10);
    ui->textBrowserTree->insertHtml(html);
}

