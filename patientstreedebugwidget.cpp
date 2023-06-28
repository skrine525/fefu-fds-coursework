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
