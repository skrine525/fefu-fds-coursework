#ifndef APPOINTMENTSTREEDEBUGWIDGET_H
#define APPOINTMENTSTREEDEBUGWIDGET_H

#include "table3.h"

#include <QWidget>

namespace Ui {
class AppointmentsTreeDebugWidget;
}

class AppointmentsTreeDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentsTreeDebugWidget(QWidget *parent = nullptr);
    ~AppointmentsTreeDebugWidget();
    void setAppointments(table3::Appointments *appointments);

private slots:
    void on_pushButtonShowDoctorPhoneNumberTree_clicked();

    void on_pushButtonShowPatientPhoneNumberTree_clicked();

    void on_pushButtonShowAppointmentDatetimeTree_clicked();

    void on_pushButtonShowAppointmentCostTree_clicked();

private:
    Ui::AppointmentsTreeDebugWidget *ui;
    table3::Appointments *appointmentsPointer;
};

#endif // APPOINTMENTSTREEDEBUGWIDGET_H
