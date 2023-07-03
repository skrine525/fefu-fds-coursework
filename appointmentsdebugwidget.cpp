#include "appointmentsdebugwidget.h"
#include "ui_appointmentsdebugwidget.h"

AppointmentsDebugWidget::AppointmentsDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppointmentsDebugWidget)
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


void AppointmentsDebugWidget::on_pushButtonShowHashTable_clicked()
{
    ui->tableWidgetHashTable->clearContents();
    ui->tableWidgetHashTable->setRowCount(0);
    appointmentsPointer->phoneNumberAndDatetimeHashTable.printToQTableWidget(ui->tableWidgetHashTable);
    ui->tableWidgetHashTable->resizeColumnsToContents();
    ui->tableWidgetHashTable->horizontalHeader()->setStretchLastSection(true);
}

