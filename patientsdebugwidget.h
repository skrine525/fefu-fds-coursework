#ifndef PATIENTSDEBUGWIDGET_H
#define PATIENTSDEBUGWIDGET_H

#include "table2.h"

#include <QWidget>

namespace Ui {
class PatientsDebugWidget;
}

class PatientsDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientsDebugWidget(QWidget *parent = nullptr);
    ~PatientsDebugWidget();
    void setPatients(table2::Patients *patients);

private slots:
    void on_pushButtonShowFullNameTree_clicked();

    void on_pushButtonShowAddressTree_clicked();

    void on_pushButtonShowAgeTree_clicked();

private:
    Ui::PatientsDebugWidget *ui;
    table2::Patients *patientsPointer;
};

#endif // PATIENTSDEBUGWIDGET_H
