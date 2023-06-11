#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addappointmentdialog.h"
#include "searchappointmentdialog.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

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
    for (int i = 0; i < doctorLabels.count(); i++)
    {
        ui->tableDoctors->setColumnWidth(i, ui->tableDoctors->width() / doctorLabels.count());
    }
    ui->tableDoctors->horizontalHeader()->setStretchLastSection(true);
    ui->tableDoctors->setHorizontalHeaderLabels(doctorLabels);
    //ui->tableDoctors->resizeColumnsToContents();

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
    appointmentLabels.append("Номер врача");
    appointmentLabels.append("Номер пациента");
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

void MainWindow::addRecordToAppointments(table3::Record record)
{
    appointments.records.append(record);
    int appendedIndex = appointments.records.count() - 1;
    appointments.doctorPhoneNumberTree.insertNode(record.doctorPhoneNumber, appendedIndex);
    appointments.patientPhoneNumberTree.insertNode(record.patientPhoneNumber, appendedIndex);
    appointments.appointmentDatetimeTree.insertNode(record.appointmentDatetime, appendedIndex);
    appointments.appointmentCostTree.insertNode(record.appointmentCost, appendedIndex);

    int rowIndex = ui->tableAppointments->rowCount();
    ui->tableAppointments->insertRow(rowIndex);
    ui->tableAppointments->setItem(rowIndex, 0, new QTableWidgetItem(QString::number(record.doctorPhoneNumber)));
    ui->tableAppointments->setItem(rowIndex, 1, new QTableWidgetItem(QString::number(record.patientPhoneNumber)));
    ui->tableAppointments->setItem(rowIndex, 2, new QTableWidgetItem(QString(record.appointmentDatetime)));
    ui->tableAppointments->setItem(rowIndex, 3, new QTableWidgetItem(QString::number(record.appointmentCost)));
    ui->tableAppointments->resizeColumnsToContents();
}

void MainWindow::on_menuFileFileExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_pushButtonAppointmentsAdd_clicked()
{
    AddAppointmentDialog addAppointmentDialog(this);
    addAppointmentDialog.exec();
}


void MainWindow::on_menuDebugAppointmentsShowTree_triggered()
{
    appointmentsTreeDebugWidget.setAppointments(appointments);
    appointmentsTreeDebugWidget.show();
}

void MainWindow::on_menuFileOpen_triggered()
{
    // Получение пути к текстовому файлу
    QString fileName =
            QFileDialog::getOpenFileName(this, "Откройте текстовый файл",
                                         QDir::homePath(), "Текстовый файл (*.txt)");

    // Создаем объект класса QFile и открываем файл для чтения
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() <<"huy";
        // Создаем объект класса QTextStream и связываем его с файлом
        QTextStream stream(&file);

        // Читаем данные из потока и выводим их на консоль
        QStringList splittedLine;
        int tableNumber = 0;
        unsigned lineCountToRead;
        while (!stream.atEnd()) {
            splittedLine = stream.readLine().split(" ");

            if(splittedLine.count() == 2)
            {
                if(splittedLine[0] == "__TABLE1__")
                    tableNumber = 1;
                else if(splittedLine[0] == "__TABLE2__")
                    tableNumber = 2;
                else if(splittedLine[0] == "__TABLE3__")
                    tableNumber = 3;
                lineCountToRead = splittedLine[1].toUInt();
            }
            else
            {
                if(tableNumber == 1)
                {

                }
                else if(tableNumber == 2)
                {

                }
                else if(tableNumber == 3 && splittedLine.count() == 8)
                {
                    if(lineCountToRead > 0)
                    {
                        lineCountToRead--;
                        table3::Record record;
                        record.doctorPhoneNumber = splittedLine[0].toLongLong();
                        record.patientPhoneNumber = splittedLine[1].toLongLong();
                        record.appointmentDatetime.year = splittedLine[2].toUInt();
                        record.appointmentDatetime.month = splittedLine[3].toUInt();
                        record.appointmentDatetime.day = splittedLine[4].toUInt();
                        record.appointmentDatetime.hour = splittedLine[5].toUInt();
                        record.appointmentDatetime.minute = splittedLine[6].toUInt();
                        record.appointmentCost = splittedLine[7].toUInt();

                        this->addRecordToAppointments(record);
                    }
                }
            }
        }

        // Закрываем файл
        file.close();
    }
}

void MainWindow::on_menuFileSave_triggered()
{
    // Получение пути к текстовому файлу
    QString fileName =
            QFileDialog::getSaveFileName(this, "Сохранить в текстовый файл",
                                         QDir::homePath(), "Текстовый файл (*.txt)");

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Создаем объект класса QTextStream и связываем его с файлом
        QTextStream stream(&file);

        // Временно, пока нет первых двух таблиц
        stream << "__TABLE1__ 0\n";
        stream << "__TABLE2__ 0\n";

        // Записываем данные в поток
        int appointmentsCount = appointments.records.count();
        stream << "__TABLE3__ " << appointmentsCount;
        for(int i = 0; i < appointmentsCount; i++)
        {
            stream << "\n";
            stream << appointments.records[i].doctorPhoneNumber << " ";
            stream << appointments.records[i].patientPhoneNumber << " ";

            stream << appointments.records[i].appointmentDatetime.year << " ";
            stream << appointments.records[i].appointmentDatetime.month << " ";
            stream << appointments.records[i].appointmentDatetime.day << " ";
            stream << appointments.records[i].appointmentDatetime.hour << " ";
            stream << appointments.records[i].appointmentDatetime.minute << " ";

            stream << appointments.records[i].appointmentCost;
        }

        // Закрываем файл
        file.close();
    }
}


void MainWindow::on_pushButtonAppointmentsSearch_clicked()
{
    SearchAppointmentDialog searchAppointmentDialog(this);
    searchAppointmentDialog.exec();
}

