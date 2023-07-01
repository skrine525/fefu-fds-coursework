#ifndef PATIENTSTREEDEBUGWIDGET_H
#define PATIENTSTREEDEBUGWIDGET_H

#include "table2.h"

#include <QWidget>

namespace Ui {
class PatientsTreeDebugWidget;
}

class PatientsTreeDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientsTreeDebugWidget(QWidget *parent = nullptr);
    ~PatientsTreeDebugWidget();
    void setPatients(table2::Patients *patients);

private slots:
    void on_pushButtonShowFullnameTree_clicked();

    void on_pushButtonShowAddressTree_clicked();

    void on_pushButtonShowAgeTree_clicked();

private:
    Ui::PatientsTreeDebugWidget *ui;
    table2::Patients *patientsPointer;
};

#endif // PATIENTSTREEDEBUGWIDGET_H
