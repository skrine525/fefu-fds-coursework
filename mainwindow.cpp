#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализация столбцов таблицы Доктора
    ui->tableDoctors->setColumnCount(4);
    QStringList doctorLabels;
    doctorLabels.append("ФИО");
    doctorLabels.append("Специальность");
    doctorLabels.append("Стаж");
    doctorLabels.append("Номер телефона");
    ui->tableDoctors->horizontalHeader()->setStretchLastSection(true);
    ui->tableDoctors->setHorizontalHeaderLabels(doctorLabels);
    ui->tableDoctors->resizeColumnsToContents();

    // Инициализация столбцов таблицы Пациенты
    ui->tablePatients->setColumnCount(4);
    QStringList patientLabels;
    patientLabels.append("ФИО");
    patientLabels.append("Регион, район");
    patientLabels.append("Возраст");
    patientLabels.append("Номер телефона");
    ui->tablePatients->horizontalHeader()->setStretchLastSection(true);
    ui->tablePatients->setHorizontalHeaderLabels(patientLabels);
    ui->tablePatients->resizeColumnsToContents();

    // Инициализация столбцов таблицы Записи
    ui->tableAppointments->setColumnCount(4);
    QStringList appointmentLabels;
    appointmentLabels.append("ФИО Врача");
    appointmentLabels.append("Фио Пациента");
    appointmentLabels.append("Время и Дата приема");
    appointmentLabels.append("Стоимость");
    ui->tableAppointments->horizontalHeader()->setStretchLastSection(true);
    ui->tableAppointments->setHorizontalHeaderLabels(appointmentLabels);
    ui->tableAppointments->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionFileExit_triggered()
{
    QApplication::quit();
}

