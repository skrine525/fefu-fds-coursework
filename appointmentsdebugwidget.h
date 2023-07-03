#ifndef APPOINTMENTSDEBUGWIDGET_H
#define APPOINTMENTSDEBUGWIDGET_H

#include "table3.h"

#include <QWidget>

namespace Ui {
class AppointmentsDebugWidget;
}

class AppointmentsDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentsDebugWidget(QWidget *parent = nullptr);
    ~AppointmentsDebugWidget();
    void setAppointments(table3::Appointments *appointments);

private slots:
    void on_pushButtonShowDoctorPhoneNumberTree_clicked();

    void on_pushButtonShowPatientPhoneNumberTree_clicked();

    void on_pushButtonShowAppointmentDatetimeTree_clicked();

    void on_pushButtonShowAppointmentCostTree_clicked();

    void on_pushButtonShowHashTable_clicked();

private:
    Ui::AppointmentsDebugWidget *ui;
    table3::Appointments *appointmentsPointer;
};

#endif // APPOINTMENTSDEBUGWIDGET_H
