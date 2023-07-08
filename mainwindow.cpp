#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adddoctordialog.h"
#include "addpatientdialog.h"
#include "addappointmentdialog.h"
#include "searchdoctordialog.h"
#include "searchpatientdialog.h"
#include "searchappointmentdialog.h"

#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализация таблицы Доктора
    ui->tableWidgetDoctors->setColumnCount(4);
    QStringList doctorLabels;
    doctorLabels.append("ФИО");
    doctorLabels.append("Специальность");
    doctorLabels.append("Стаж");
    doctorLabels.append("Номер телефона");
    ui->tableWidgetDoctors->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetDoctors->setHorizontalHeaderLabels(doctorLabels);
    ui->tableWidgetDoctors->resizeColumnsToContents();

    // Инициализация таблицы Пациенты
    ui->tableWidgetPatients->setColumnCount(5);
    QStringList patientLabels;
    patientLabels.append("ФИО");
    patientLabels.append("Регион");
    patientLabels.append("Район");
    patientLabels.append("Возраст");
    patientLabels.append("Номер телефона");
    ui->tableWidgetPatients->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetPatients->setHorizontalHeaderLabels(patientLabels);
    ui->tableWidgetPatients->resizeColumnsToContents();

    // Инициализация таблицы Записи
    ui->tableWidgetAppointments->setColumnCount(4);
    QStringList appointmentLabels;
    appointmentLabels.append("Номер врача");
    appointmentLabels.append("Номер пациента");
    appointmentLabels.append("Дата и Время приема");
    appointmentLabels.append("Стоимость");
    ui->tableWidgetAppointments->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetAppointments->setHorizontalHeaderLabels(appointmentLabels);
    ui->tableWidgetAppointments->resizeColumnsToContents();

    // Делаем перезагрузку интерфейса и данных
    this->resetViewAndData();

    // Передаем ссылки на таблицы в соответствующие меню отладки
    doctorsDebugWidget.setDoctors(&doctors);
    patientsDebugWidget.setPatients(&patients);
    appointmentsDebugWidget.setAppointments(&appointments);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetViewAndData()
{
    // Очищаем визуальную часть
    ui->tableWidgetDoctors->clearContents();
    ui->tableWidgetDoctors->setRowCount(0);
    ui->tableWidgetPatients->clearContents();
    ui->tableWidgetPatients->setRowCount(0);
    ui->tableWidgetAppointments->clearContents();
    ui->tableWidgetAppointments->setRowCount(0);

    // Отключаем кнопку Очистка поиска
    ui->pushButtonDoctorsClearSearch->setEnabled(false);
    ui->pushButtonPatientsClearSearch->setEnabled(false);
    ui->pushButtonAppointmentsClearSearch->setEnabled(false);

    // Очищаем векторы и структуры данных
    doctors.records.clear();
    doctors.experienceTree.clear();
    doctors.fullNameTree.clear();
    doctors.specialityTree.clear();
    doctors.phoneNumberHashTable.clear();
    patients.records.clear();
    patients.addressTree.clear();
    patients.ageTree.clear();
    patients.fullNameTree.clear();
    patients.phoneNumberHashTable.clear();
    appointments.records.clear();
    appointments.doctorPhoneNumberTree.clear();
    appointments.patientPhoneNumberTree.clear();
    appointments.appointmentDatetimeTree.clear();
    appointments.appointmentCostTree.clear();
    appointments.phoneNumberAndDatetimeHashTable.clear();
}

int MainWindow::addRecordToAppointments(table3::Record record)
{
    // Возвращает 0, если вставка прошла успешно
    // Возвращает 1, если не удалось вставить ключ в ХТ
    // Возвращает 2, если не найден доктор в ХТ
    // Возвращает 3, если не найден пациент в ХТ

    // Проверка целостности
    if(!doctors.phoneNumberHashTable.find(record.doctorPhoneNumber))
        return 2;
    if(!patients.phoneNumberHashTable.find(record.patientPhoneNumber))
        return 3;

    // Вставляем в хеш-таблицу
    int appendedIndex = appointments.records.count();
    table3::PhoneNumberAndDatetime pnd;
    pnd.doctorPhoneNumber = record.doctorPhoneNumber;
    pnd.patientPhoneNumber = record.patientPhoneNumber;
    pnd.appointmentDatetime = record.appointmentDatetime;
    if(!appointments.phoneNumberAndDatetimeHashTable.insert(pnd, appendedIndex))
        return 1;

    // Заносим запись в вектор и добавляем в структуры данных
    appointments.records.append(record);
    appointments.doctorPhoneNumberTree.insertNode(record.doctorPhoneNumber, appendedIndex);
    appointments.patientPhoneNumberTree.insertNode(record.patientPhoneNumber, appendedIndex);
    appointments.appointmentDatetimeTree.insertNode(record.appointmentDatetime, appendedIndex);
    appointments.appointmentCostTree.insertNode(record.appointmentCost, appendedIndex);

    // Элементы строки
    QTableWidgetItem *doctorPhoneNumberItem = new QTableWidgetItem(QString::number(record.doctorPhoneNumber));
    QTableWidgetItem *patientPhoneNumberItem = new QTableWidgetItem(QString::number(record.patientPhoneNumber));
    QTableWidgetItem *appointmentDatetimeItem = new QTableWidgetItem(QString(record.appointmentDatetime));
    QTableWidgetItem *appointmentCostItem = new QTableWidgetItem(QString::number(record.appointmentCost));

    // Устанавливаем флаг запрета редактирования для каждого элемента
    doctorPhoneNumberItem->setFlags(doctorPhoneNumberItem->flags() & ~Qt::ItemIsEditable);
    patientPhoneNumberItem->setFlags(patientPhoneNumberItem->flags() & ~Qt::ItemIsEditable);
    appointmentDatetimeItem->setFlags(appointmentDatetimeItem->flags() & ~Qt::ItemIsEditable);
    appointmentCostItem->setFlags(appointmentCostItem->flags() & ~Qt::ItemIsEditable);

    // Заносим строку в таблицу
    int rowIndex = ui->tableWidgetAppointments->rowCount();
    ui->tableWidgetAppointments->insertRow(rowIndex);
    ui->tableWidgetAppointments->setItem(rowIndex, 0, doctorPhoneNumberItem);
    ui->tableWidgetAppointments->setItem(rowIndex, 1, patientPhoneNumberItem);
    ui->tableWidgetAppointments->setItem(rowIndex, 2, appointmentDatetimeItem);
    ui->tableWidgetAppointments->setItem(rowIndex, 3, appointmentCostItem);
    ui->tableWidgetPatients->resizeColumnsToContents();

    return 0;
}

bool MainWindow::addRecordToPatients(table2::Record record)
{
    // Вставляем в хеш-таблицу
    int appendedIndex = patients.records.count();
    int hashTableResult = patients.phoneNumberHashTable.insert(table2::HashTableEntry(record.phoneNumber, appendedIndex));
    if(hashTableResult == 1)
    {
        QMessageBox::warning(this, "Внимание", "Такой номер телефона уже добавлен.");
        return false;
    }
    else if(hashTableResult == 2)
    {
        QMessageBox::warning(this, "Внимание", "Таблица переполнена.");
        return false;
    }

    // Форматирование строковых данных
    QStringList fullnameList = record.fullName.split(" ");
    QString lastName = fullnameList[0][0].toUpper() + fullnameList[0].mid(1).toLower();
    QString firstName = fullnameList[1][0].toUpper() + fullnameList[1].mid(1).toLower();
    QString middleName = fullnameList[2][0].toUpper() + fullnameList[2].mid(1).toLower();
    record.fullName = QString("%1 %2 %3").arg(lastName, firstName, middleName);
    record.region = record.region[0].toUpper() + record.region.mid(1).toLower();
    record.district = record.district[0].toUpper() + record.district.mid(1).toLower();


    // Заносим запись в вектор и добавляем в структуры данных
    patients.records.append(record);
    patients.addressTree.insertNode(table2::Address(record.region, record.district), appendedIndex);
    patients.ageTree.insertNode(record.age, appendedIndex);
    patients.fullNameTree.insertNode(record.fullName, appendedIndex);

    // Элементы строки
    QTableWidgetItem *fullNameItem = new QTableWidgetItem(record.fullName);
    QTableWidgetItem *regionItem = new QTableWidgetItem(record.region);
    QTableWidgetItem *districtItem = new QTableWidgetItem(record.district);
    QTableWidgetItem *ageItem = new QTableWidgetItem(QString::number(record.age));
    QTableWidgetItem *phoneNumberItem = new QTableWidgetItem(QString::number(record.phoneNumber));

    // Устанавливаем флаг запрета редактирования для каждого элемента
    fullNameItem->setFlags(fullNameItem->flags() & ~Qt::ItemIsEditable);
    regionItem->setFlags(regionItem->flags() & ~Qt::ItemIsEditable);
    districtItem->setFlags(districtItem->flags() & ~Qt::ItemIsEditable);
    ageItem->setFlags(ageItem->flags() & ~Qt::ItemIsEditable);
    phoneNumberItem->setFlags(phoneNumberItem->flags() & ~Qt::ItemIsEditable);

    // Заносим строку в таблицу
    int rowIndex = ui->tableWidgetPatients->rowCount();
    ui->tableWidgetPatients->insertRow(rowIndex);
    ui->tableWidgetPatients->setItem(rowIndex, 0, fullNameItem);
    ui->tableWidgetPatients->setItem(rowIndex, 1, regionItem);
    ui->tableWidgetPatients->setItem(rowIndex, 2, districtItem);
    ui->tableWidgetPatients->setItem(rowIndex, 3, ageItem);
    ui->tableWidgetPatients->setItem(rowIndex, 4, phoneNumberItem);
    ui->tableWidgetPatients->resizeColumnsToContents();

    return true;
}

bool MainWindow::addRecordToDoctors(table1::Record record)
{
    // Вставляем в хеш-таблицу
    int appendedIndex = doctors.records.count();
    int hashTableResult = doctors.phoneNumberHashTable.insert(table1::HashTableEntry(record.phoneNumber, appendedIndex));
    if(hashTableResult == 1)
    {
        QMessageBox::warning(this, "Внимание", "Такой номер телефона уже добавлен.");
        return false;
    }
    else if(hashTableResult == 2)
    {
        QMessageBox::warning(this, "Внимание", "Таблица переполнена.");
        return false;
    }

    // Форматирование строковых данных
    QStringList fullnameList = record.fullName.split(" ");
    QString lastName = fullnameList[0][0].toUpper() + fullnameList[0].mid(1).toLower();
    QString firstName = fullnameList[1][0].toUpper() + fullnameList[1].mid(1).toLower();
    QString middleName = fullnameList[2][0].toUpper() + fullnameList[2].mid(1).toLower();
    record.fullName = QString("%1 %2 %3").arg(lastName, firstName, middleName);
    record.speciality = record.speciality[0].toUpper() + record.speciality.mid(1).toLower();

    // Заносим запись в вектор и добавляем в структуры данных
    doctors.records.append(record);
    doctors.specialityTree.insertNode(record.speciality, appendedIndex);
    doctors.experienceTree.insertNode(record.experience, appendedIndex);
    doctors.fullNameTree.insertNode(record.fullName, appendedIndex);

    // Элементы строки
    QTableWidgetItem *fullNameItem = new QTableWidgetItem(record.fullName);
    QTableWidgetItem *specialityItem = new QTableWidgetItem(record.speciality);
    QTableWidgetItem *experienceItem = new QTableWidgetItem(QString::number(record.experience));
    QTableWidgetItem *phoneNumberItem = new QTableWidgetItem(QString::number(record.phoneNumber));

    // Устанавливаем флаг запрета редактирования для каждого элемента
    fullNameItem->setFlags(fullNameItem->flags() & ~Qt::ItemIsEditable);
    specialityItem->setFlags(specialityItem->flags() & ~Qt::ItemIsEditable);
    experienceItem->setFlags(experienceItem->flags() & ~Qt::ItemIsEditable);
    phoneNumberItem->setFlags(phoneNumberItem->flags() & ~Qt::ItemIsEditable);

    // Заносим строку в таблицу
    int rowIndex = ui->tableWidgetDoctors->rowCount();
    ui->tableWidgetDoctors->insertRow(rowIndex);
    ui->tableWidgetDoctors->setItem(rowIndex, 0, fullNameItem);
    ui->tableWidgetDoctors->setItem(rowIndex, 1, specialityItem);
    ui->tableWidgetDoctors->setItem(rowIndex, 2, experienceItem);
    ui->tableWidgetDoctors->setItem(rowIndex, 3, phoneNumberItem);
    ui->tableWidgetDoctors->resizeColumnsToContents();

    return true;
}

void MainWindow::on_pushButtonAppointmentsAdd_clicked()
{
    AddAppointmentDialog addAppointmentDialog(this);
    addAppointmentDialog.setMainWindow(this);
    addAppointmentDialog.exec();
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
        this->resetViewAndData();
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
            else if(lineCountToRead > 0)
            {
                lineCountToRead--;

                if(tableNumber == 1 && splittedLine.count() == 6)
                {
                    table1::Record record;
                    record.fullName = QString("%1 %2 %3").arg(
                                splittedLine[0], splittedLine[1], splittedLine[2]);
                    record.speciality = splittedLine[3];
                    record.experience = splittedLine[4].toUInt();
                    record.phoneNumber = splittedLine[5].toLongLong();

                    this->addRecordToDoctors(record);

                }
                else if(tableNumber == 2 && splittedLine.count() == 7)
                {
                    table2::Record record;
                    record.fullName = QString("%1 %2 %3").arg(
                                splittedLine[0], splittedLine[1], splittedLine[2]);
                    record.region = splittedLine[3];
                    record.district = splittedLine[4];
                    record.age = splittedLine[5].toUInt();
                    record.phoneNumber = splittedLine[6].toLongLong();

                    this->addRecordToPatients(record);
                }
                else if(tableNumber == 3 && splittedLine.count() == 8)
                {
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

        // Закрываем файл
        file.close();
        ui->statusbar->showMessage("Файл успешно прочитан.");
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

        // Записываем данные в поток из таблицы Доктора
        int doctorsCount = doctors.records.count();
        stream << "__TABLE1__ " << doctorsCount;
        for(int i = 0; i < doctorsCount; i++)
        {
            stream << "\n";
            stream << doctors.records[i].fullName << " ";
            stream << doctors.records[i].speciality << " ";
            stream << doctors.records[i].experience << " ";
            stream << doctors.records[i].phoneNumber;
        }

        // Записываем данные в поток из таблицы Пациенты
        int patinentsCount = patients.records.count();
        stream << "\n__TABLE2__ " << patinentsCount;
        for(int i = 0; i < patinentsCount; i++)
        {
            stream << "\n";
            stream << patients.records[i].fullName << " ";
            stream << patients.records[i].region << " ";
            stream << patients.records[i].district << " ";
            stream << patients.records[i].age << " ";
            stream << patients.records[i].phoneNumber;
        }

        // Записываем данные в поток из таблицы Записи
        int appointmentsCount = appointments.records.count();
        stream << "\n__TABLE3__ " << appointmentsCount;
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
        ui->statusbar->showMessage("Файл успешно сохранен.");
    }
}


void MainWindow::on_pushButtonAppointmentsSearch_clicked()
{
    SearchAppointmentDialog searchAppointmentDialog(this);
    searchAppointmentDialog.setMainWindow(this);
    searchAppointmentDialog.exec();
}

void MainWindow::showAppointmentSearchResult(table3::Record record, int fieldIndex)
{
    table3::DoublyLinkedRingList *valueList = nullptr;

    if(fieldIndex == 0)
    {
        // Номер врача
        auto *node = appointments.doctorPhoneNumberTree.findNode(record.doctorPhoneNumber);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 1)
    {
        // Номер пациента
        auto *node = appointments.patientPhoneNumberTree.findNode(record.patientPhoneNumber);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 2)
    {
        // Дата и Время приёма
        auto *node = appointments.appointmentDatetimeTree.findNode(record.appointmentDatetime);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 3)
    {
        // Стоимость
        auto *node = appointments.appointmentCostTree.findNode(record.appointmentCost);
        if(node != nullptr)
            valueList = node->valueList;
    }

    // Отображение поиска
    if(valueList == nullptr)
    {
        ui->pushButtonAppointmentsClearSearch->setEnabled(true);
        int rowCount = ui->tableWidgetAppointments->rowCount();
        for(int i = 0; i < rowCount; i++)
            ui->tableWidgetAppointments->hideRow(i);
        ui->statusbar->showMessage("Записи - Найдено 0 элемент(ов).");
    }
    else
    {
        // Включаем кнопку Очистить поиск
        ui->pushButtonAppointmentsClearSearch->setEnabled(true);

        auto *head = valueList->getHead();
        auto *curr = head;
        if (curr != nullptr)
        {
            int rowCount = ui->tableWidgetAppointments->rowCount();
            for(int i = 0; i < rowCount; i++)
                ui->tableWidgetAppointments->hideRow(i);

            int indexCount = 0;
            do
            {
                indexCount++;
                ui->tableWidgetAppointments->showRow(curr->value);
                curr = curr->next;
            } while (curr != head);
            ui->statusbar->showMessage(QString("Записи - Найдено %1 элемент(ов).").arg(indexCount));
        }
    }
}

void MainWindow::showDoctorSearchResult(table1::Record record, int fieldIndex)
{
    table1::DoubleLinkedList *valueList = nullptr;

    if(fieldIndex == 0)
    {
        // ФИО

        auto *node = doctors.fullNameTree.findNode(record.fullName);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 1)
    {
        // Специальность

        auto *node = doctors.specialityTree.findNode(record.speciality);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 2)
    {
        // Стаж

        auto *node = doctors.experienceTree.findNode(record.experience);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 3)
    {
        // Номер телефона

        auto entry = doctors.phoneNumberHashTable.find(record.phoneNumber);

        // Скрываем все строки таблицы
        int rowCount = ui->tableWidgetDoctors->rowCount();
        for(int i = 0; i < rowCount; i++)
            ui->tableWidgetDoctors->hideRow(i);

        int indexCount = 0;
        if(entry)
        {
            ui->tableWidgetDoctors->showRow(entry->value);
            indexCount++;
        }

        ui->statusbar->showMessage(QString("Доктора - Найдено %1 элемент(ов).").arg(indexCount));
        ui->pushButtonDoctorsClearSearch->setEnabled(true);
        return;
    }

    // Отображение поиска
    if(valueList == nullptr)
    {
        ui->pushButtonDoctorsClearSearch->setEnabled(true);
        int rowCount = ui->tableWidgetDoctors->rowCount();
        for(int i = 0; i < rowCount; i++)
            ui->tableWidgetDoctors->hideRow(i);
        ui->statusbar->showMessage("Доктора - Найдено 0 элемент(ов).");
    }
    else
    {
        // Включаем кнопку Очистить поиск
        ui->pushButtonDoctorsClearSearch->setEnabled(true);

        auto *curr = valueList->getHead();
        if (curr != nullptr)
        {
            int rowCount = ui->tableWidgetDoctors->rowCount();
            for(int i = 0; i < rowCount; i++)
                ui->tableWidgetDoctors->hideRow(i);

            int indexCount = 0;
            do
            {
                indexCount++;
                ui->tableWidgetDoctors->showRow(curr->value);
                curr = curr->next;
            }
            while (curr != nullptr);
            ui->statusbar->showMessage(QString("Доктора - Найдено %1 элемент(ов).").arg(indexCount));
        }
    }
}

void MainWindow::showPatientSearchResult(table2::Record record, int fieldIndex)
{
    table2::SingleLinkedList *valueList = nullptr;

    if(fieldIndex == 0)
    {
        // ФИО

        auto *node = patients.fullNameTree.findNode(record.fullName);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 1)
    {
        // Адрес

        auto *node = patients.addressTree.findNode(table2::Address(record.region, record.district));
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 2)
    {
        // Возраст

        auto *node = patients.ageTree.findNode(record.age);
        if(node != nullptr)
            valueList = node->valueList;
    }
    else if(fieldIndex == 3)
    {
        // Номер телефона

        auto entry = patients.phoneNumberHashTable.find(record.phoneNumber);

        // Скрываем все строки таблицы
        int rowCount = ui->tableWidgetPatients->rowCount();
        for(int i = 0; i < rowCount; i++)
            ui->tableWidgetPatients->hideRow(i);

        int indexCount = 0;
        if(entry)
        {
            ui->tableWidgetPatients->showRow(entry->value);
            indexCount++;
        }

        ui->statusbar->showMessage(QString("Пациенты - Найдено %1 элемент(ов).").arg(indexCount));
        ui->pushButtonPatientsClearSearch->setEnabled(true);
        return;
    }

    // Отображение поиска
    if(valueList == nullptr)
    {
        ui->pushButtonPatientsClearSearch->setEnabled(true);
        int rowCount = ui->tableWidgetDoctors->rowCount();
        for(int i = 0; i < rowCount; i++)
            ui->tableWidgetPatients->hideRow(i);
        ui->statusbar->showMessage("Пациенты - Найдено 0 элемент(ов).");
    }
    else
    {
        // Включаем кнопку Очистить поиск
        ui->pushButtonPatientsClearSearch->setEnabled(true);

        auto *curr = valueList->getHead();
        if (curr != nullptr)
        {
            int rowCount = ui->tableWidgetPatients->rowCount();
            for(int i = 0; i < rowCount; i++)
                ui->tableWidgetPatients->hideRow(i);

            int indexCount = 0;
            while (curr != nullptr)
            {
                indexCount++;
                ui->tableWidgetPatients->showRow(curr->value);
                curr = curr->next;
            }
            ui->statusbar->showMessage(QString("Пациенты - Найдено %1 элемент(ов).").arg(indexCount));
        }
    }
}

void MainWindow::on_pushButtonAppointmentsClearSearch_clicked()
{
    // Выключаем кнопку Очистить поиск
    ui->pushButtonAppointmentsClearSearch->setEnabled(false);
    int rowCount = ui->tableWidgetAppointments->rowCount();
    for(int i = 0; i < rowCount; i++)
        ui->tableWidgetAppointments->showRow(i);
    ui->statusbar->showMessage("Записи - Поиск очищен.");
}

void MainWindow::on_menuFileCreate_triggered()
{
    this->resetViewAndData();
}

void MainWindow::on_pushButtonAppointmentsDelete_clicked()
{
//    int rowIndex = ui->tableWidgetAppointments->currentRow();
//    if(rowIndex != -1)
//    {
//        table3::Record record = appointments.records[rowIndex];
//        appointments.doctorPhoneNumberTree.removeNode(record.doctorPhoneNumber, rowIndex);
//        appointments.patientPhoneNumberTree.removeNode(record.patientPhoneNumber, rowIndex);
//        appointments.appointmentDatetimeTree.removeNode(record.appointmentDatetime, rowIndex);
//        appointments.appointmentCostTree.removeNode(record.appointmentCost, rowIndex);
//        appointments.records.remove(rowIndex);
//        ui->tableWidgetAppointments->removeRow(rowIndex);
//    }
//    else
//        QMessageBox::warning(this, "Внимание", "Для удаления записи необходимо выбрать строку.");
}

void MainWindow::on_pushButtonPatientsAdd_clicked()
{
    AddPatientDialog addPatientDialog(this);
    addPatientDialog.setMainWindow(this);
    addPatientDialog.exec();
}

void MainWindow::on_pushButtonDoctorsAdd_clicked()
{
    AddDoctorDialog addDoctorDialog(this);
    addDoctorDialog.setMainWindow(this);
    addDoctorDialog.exec();
}

void MainWindow::on_menuDebugDoctors_triggered()
{
    doctorsDebugWidget.show();
}

void MainWindow::on_menuDebugPatients_triggered()
{
    patientsDebugWidget.show();
}

void MainWindow::on_menuFileExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_menuDebugAppointments_triggered()
{
    appointmentsDebugWidget.show();
}

void MainWindow::on_pushButtonDoctorsSearch_clicked()
{
    SearchDoctorDialog searchDoctorDialog(this);
    searchDoctorDialog.setMainWindow(this);
    searchDoctorDialog.exec();
}

void MainWindow::on_pushButtonDoctorsClearSearch_clicked()
{
    // Выключаем кнопку Очистить поиск
    ui->pushButtonDoctorsClearSearch->setEnabled(false);
    int rowCount = ui->tableWidgetDoctors->rowCount();
    for(int i = 0; i < rowCount; i++)
        ui->tableWidgetDoctors->showRow(i);
    ui->statusbar->showMessage("Доктора - Поиск очищен.");
}

void MainWindow::on_pushButtonPatientsSeatch_clicked()
{
    SearchPatientDialog searchPatientDialog(this);
    searchPatientDialog.setMainWindow(this);
    searchPatientDialog.exec();
}

void MainWindow::on_pushButtonPatientsClearSearch_clicked()
{
    // Выключаем кнопку Очистить поиск
    ui->pushButtonPatientsClearSearch->setEnabled(false);
    int rowCount = ui->tableWidgetPatients->rowCount();
    for(int i = 0; i < rowCount; i++)
        ui->tableWidgetPatients->showRow(i);
    ui->statusbar->showMessage("Доктора - Поиск очищен.");
}

void MainWindow::on_pushButtonDoctorsDelete_clicked()
{
    int rowIndex = ui->tableWidgetDoctors->currentRow();
    if(rowIndex != -1)
    {
        QString fullnameString = doctors.records[rowIndex].fullName;
        QString phoneNumberString = QString::number(doctors.records[rowIndex].phoneNumber);

        QMessageBox msgBox;
        msgBox.setWindowTitle("Доктора - Удаление");
        msgBox.setText("Вы точно хотите удалить доктора?\n\nФИО: " + fullnameString
                       + "\nНомер телефона: " + phoneNumberString);
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
        {
            if(removeRecordFromDoctors(rowIndex))
                ui->tableWidgetDoctors->setCurrentCell(-1, -1);
            else
                QMessageBox::warning(this, "Внимание", "Во время удаления произошла ошибка в таблице.");
        }
    }
    else
        QMessageBox::warning(this, "Внимание", "Для удаления Доктора необходимо выбрать строку.");
}

bool MainWindow::removeRecordFromDoctors(int index)
{
    if(doctors.records.length() > 0)
    {
        int lastIndex = doctors.records.length() - 1;
        if(0 <= index && index < lastIndex)
        {
            // Удаление записи
            table1::Record record = doctors.records[index];
            doctors.experienceTree.removeNode(record.experience, index);
            doctors.fullNameTree.removeNode(record.fullName, index);
            doctors.specialityTree.removeNode(record.speciality, index);
            doctors.phoneNumberHashTable.remove(table1::HashTableEntry(record.phoneNumber, index));

            // Ставим поледнюю запись на место удаляемой, и удаляем последнюю из вектора и UI
            doctors.records[index] = doctors.records[lastIndex];
            doctors.records.remove(lastIndex);
            record = doctors.records[index];

            // Обновляем структуры данных для замены
            auto experienceTreeNode = doctors.experienceTree.findNode(record.experience);
            experienceTreeNode->valueList->removeNode(lastIndex);
            experienceTreeNode->valueList->insertNode(index);
            auto fullNameTreeNode = doctors.fullNameTree.findNode(record.fullName);
            fullNameTreeNode->valueList->removeNode(lastIndex);
            fullNameTreeNode->valueList->insertNode(index);
            auto specialityTreeNode = doctors.specialityTree.findNode(record.speciality);
            specialityTreeNode->valueList->removeNode(lastIndex);
            specialityTreeNode->valueList->insertNode(index);
            auto hashTableEntry = doctors.phoneNumberHashTable.find(record.phoneNumber);
            hashTableEntry->value = index;

            // Обновляем интерфейс
            ui->tableWidgetDoctors->removeRow(lastIndex);

            // Элементы строки
            QTableWidgetItem *fullNameItem = new QTableWidgetItem(record.fullName);
            QTableWidgetItem *specialityItem = new QTableWidgetItem(record.speciality);
            QTableWidgetItem *experienceItem = new QTableWidgetItem(QString::number(record.experience));
            QTableWidgetItem *phoneNumberItem = new QTableWidgetItem(QString::number(record.phoneNumber));

            // Устанавливаем флаг запрета редактирования для каждого элемента
            fullNameItem->setFlags(fullNameItem->flags() & ~Qt::ItemIsEditable);
            specialityItem->setFlags(specialityItem->flags() & ~Qt::ItemIsEditable);
            experienceItem->setFlags(experienceItem->flags() & ~Qt::ItemIsEditable);
            phoneNumberItem->setFlags(phoneNumberItem->flags() & ~Qt::ItemIsEditable);

            // Заносим строку в таблицу
            ui->tableWidgetDoctors->setItem(index, 0, fullNameItem);
            ui->tableWidgetDoctors->setItem(index, 1, specialityItem);
            ui->tableWidgetDoctors->setItem(index, 2, experienceItem);
            ui->tableWidgetDoctors->setItem(index, 3, phoneNumberItem);
            ui->tableWidgetDoctors->resizeColumnsToContents();

            return true;
        }
        else if(index == lastIndex)
        {
            qDebug() << "Удаление последнего " << index;
            // Удаление записи
            table1::Record record = doctors.records[index];
            doctors.experienceTree.removeNode(record.experience, index);
            doctors.fullNameTree.removeNode(record.fullName, index);
            doctors.specialityTree.removeNode(record.speciality, index);
            doctors.phoneNumberHashTable.remove(table1::HashTableEntry(record.phoneNumber, index));
            doctors.records.remove(index);
            ui->tableWidgetDoctors->removeRow(index);

            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void MainWindow::on_pushButtonPatientsDelete_clicked()
{
    int rowIndex = ui->tableWidgetPatients->currentRow();
    if(rowIndex != -1)
    {
        QString fullnameString = patients.records[rowIndex].fullName;
        QString phoneNumberString = QString::number(patients.records[rowIndex].phoneNumber);

        QMessageBox msgBox;
        msgBox.setWindowTitle("Пациенты - Удаление");
        msgBox.setText("Вы точно хотите удалить пациента?\n\nФИО: " + fullnameString
                       + "\nНомер телефона: " + phoneNumberString);
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
        {
            if(removeRecordFromPatients(rowIndex))
                ui->tableWidgetDoctors->setCurrentCell(-1, -1);
            else
                QMessageBox::warning(this, "Внимание", "Во время удаления произошла ошибка.");
        }
    }
    else
        QMessageBox::warning(this, "Внимание", "Для удаления Пациента необходимо выбрать строку в таблице.");
}

bool MainWindow::removeRecordFromPatients(int index)
{
    if(patients.records.length() > 0)
    {
        int lastIndex = patients.records.length() - 1;
        if(0 <= index && index < lastIndex)
        {
            // Удаление записи
            table2::Record record = patients.records[index];
            patients.addressTree.removeNode(table2::Address(record.region, record.district), index);
            patients.fullNameTree.removeNode(record.fullName, index);
            patients.phoneNumberHashTable.remove(table2::HashTableEntry(record.phoneNumber, index));

            // Ставим поледнюю запись на место удаляемой, и удаляем последнюю из вектора и UI
            patients.records[index] = patients.records[lastIndex];
            patients.records.remove(lastIndex);
            record = patients.records[index];

            // Обновляем структуры данных для замены
            auto addressTreeNode = patients.addressTree.findNode(table2::Address(record.region, record.district));
            addressTreeNode->valueList->removeNode(lastIndex);
            addressTreeNode->valueList->insertNode(index);
            auto fullNameTreeNode = patients.fullNameTree.findNode(record.fullName);
            fullNameTreeNode->valueList->removeNode(lastIndex);
            fullNameTreeNode->valueList->insertNode(index);
            auto ageTreeNode = patients.ageTree.findNode(record.age);
            ageTreeNode->valueList->removeNode(lastIndex);
            ageTreeNode->valueList->insertNode(index);
            auto hashTableEntry = patients.phoneNumberHashTable.find(record.phoneNumber);
            hashTableEntry->value = index;

            // Обновляем интерфейс
            ui->tableWidgetPatients->removeRow(lastIndex);

            // Элементы строки
            QTableWidgetItem *fullNameItem = new QTableWidgetItem(record.fullName);
            QTableWidgetItem *regionItem = new QTableWidgetItem(record.region);
            QTableWidgetItem *districtItem = new QTableWidgetItem(record.district);
            QTableWidgetItem *ageItem = new QTableWidgetItem(QString::number(record.age));
            QTableWidgetItem *phoneNumberItem = new QTableWidgetItem(QString::number(record.phoneNumber));

            // Устанавливаем флаг запрета редактирования для каждого элемента
            fullNameItem->setFlags(fullNameItem->flags() & ~Qt::ItemIsEditable);
            regionItem->setFlags(regionItem->flags() & ~Qt::ItemIsEditable);
            districtItem->setFlags(districtItem->flags() & ~Qt::ItemIsEditable);
            ageItem->setFlags(ageItem->flags() & ~Qt::ItemIsEditable);
            phoneNumberItem->setFlags(phoneNumberItem->flags() & ~Qt::ItemIsEditable);

            // Заносим строку в таблицу
            ui->tableWidgetPatients->setItem(index, 0, fullNameItem);
            ui->tableWidgetPatients->setItem(index, 1, regionItem);
            ui->tableWidgetPatients->setItem(index, 2, districtItem);
            ui->tableWidgetPatients->setItem(index, 3, ageItem);
            ui->tableWidgetPatients->setItem(index, 4, phoneNumberItem);
            ui->tableWidgetPatients->resizeColumnsToContents();

            return true;
        }
        else if(index == lastIndex)
        {
            qDebug() << "Удаление последнего " << index;
            // Удаление записи
            table2::Record record = patients.records[index];
            patients.addressTree.removeNode(table2::Address(record.region, record.district), index);
            patients.ageTree.removeNode(record.age, index);
            patients.phoneNumberHashTable.remove(table2::HashTableEntry(record.phoneNumber, index));
            patients.records.remove(index);
            ui->tableWidgetPatients->removeRow(index);

            return true;
        }
        else
            return false;
    }
    else
        return false;
}
