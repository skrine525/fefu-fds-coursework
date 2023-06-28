#ifndef PATIENTSTREEDEBUGWIDGET_H
#define PATIENTSTREEDEBUGWIDGET_H

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

private:
    Ui::PatientsTreeDebugWidget *ui;
};

#endif // PATIENTSTREEDEBUGWIDGET_H
