#ifndef DOCTORSDEBUGWIDGET_H
#define DOCTORSDEBUGWIDGET_H

#include "table1.h"

#include <QWidget>

namespace Ui {
class DoctorsDebugWidget;
}

class DoctorsDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorsDebugWidget(QWidget *parent = nullptr);
    ~DoctorsDebugWidget();
    void setDoctors(table1::Doctors *doctors);

private slots:
    void on_pushButtonShowFullNameTree_clicked();

    void on_pushButtonShowSpecialityTree_clicked();

    void on_pushButtonShowExperienceTree_clicked();

    void on_pushButtonShowHashTable_clicked();

private:
    Ui::DoctorsDebugWidget *ui;
    table1::Doctors *doctorsPointer;
};

#endif // DOCTORSDEBUGWIDGET_H
