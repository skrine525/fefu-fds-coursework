#include "table2.h"

#include <QString>

table2::Address::operator QString() const
{
    return QString("%1, %2").arg(region, district);
}

bool table2::operator==(HashTableEntry &a, HashTableEntry &b)
{
    return a.key == b.key;
}

bool table2::operator!=(HashTableEntry &a, HashTableEntry &b)
{
    return !(a == b);
}

bool table2::operator<(table2::Address& a, table2::Address& b)
{
    if ((a.region.compare(b.region) < 0) || (a.region.compare(b.region) == 0 && a.district.compare(b.district) < 0)) return 1;
    else return 0;
}

bool table2::operator>(table2::Address& a, table2::Address& b)
{
    if ((a.region.compare(b.region) > 0) || (a.region.compare(b.region) == 0 && a.district.compare(b.district) > 0)) return 1;
    else return 0;
}

bool table2::operator==(table2::Address& a, table2::Address& b)
{
    if (a.region.compare(b.region) == 0 && a.district.compare(b.district) == 0) return 1;
    else return 0;
}

bool table2::operator!=(table2::Address& a, table2::Address& b)
{
    if (a.region.compare(b.region) != 0 && a.district.compare(b.district) != 0) return 1;
    else return 0;
}

table2::HashTableEntry::HashTableEntry(long long key, int value)
{
    this->key = key;
    this->value = value;
}

table2::Address::Address(QString region, QString district)
{
    this->region = region;
    this->district = district;
}

int table2::HashTable::firstHash(long long key, int n)
{
    QString str = QString::number(key);
    long long hash = 0;

    for (int i = 0; i < 11; i += 2)
    {
        hash += str.mid(i, 2).toInt();
    }

    return hash % n;
}

int table2::HashTable::secondHash(int firstHash, int k1, int k2, int j)
{
    return (firstHash + j * k1 + j * j * k2) % size;
}

table2::HashTable::HashTable(int maxN)
{
    size = maxN;
    table = new HashTableEntry[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = HashTableEntry();
    }
}

int table2::HashTable::solveInsertCollision(HashTableEntry key)
{
    int insertIndex = firstHash(key.key, size);
    if (table[insertIndex].status == 2)
    {
        bool canInsert = true;

        int i;
        int j = 0;
        do
        {
            j++;
            i = secondHash(key.firstHash, 1, 1, j);
            if (table[i] == key && table[i].status == 1)
                canInsert = false;

        } while (j < size && (table[i] != key || table[i].status != 1) && table[i].status != 0);

        if (canInsert)
        {
            key.secondHash = i;
            key.status = 1;
            table[insertIndex] = key;
            return 0; // Ключ вставлен
        }
        else
        {
            return 1; // Ключ уже существует
        }
    }
    else if (table[insertIndex] != key)
    {
        insertIndex = -1;

        int i;
        int j = 0;
        do
        {
            j++;
            i = secondHash(key.firstHash, 1, 1, j);

            if (insertIndex == -1 && (table[i].status == 2 || table[i].status == 0))
            {
                insertIndex = i;
            }
            else if (table[i] == key && table[i].status == 1)
            {
                insertIndex = -1;
            }

        } while (j < size && (table[i] != key || table[i].status != 1) && table[i].status != 0);

        if (insertIndex != -1)
        {
            key.status = 1;
            key.secondHash = i;
            table[insertIndex] = key;
            return 0; // Ключ вставлен
        }
        else if (table[i] == key)
        {
            return 1; // Ключ уже существует
        }
        else if (j == size)
        {
            return 2; // Таблица переполнена
        }
    }
    return 1;
}


int table2::HashTable::insert(HashTableEntry key)
{
    int insertIndex = firstHash(key.key, size);
    key.firstHash = insertIndex;
    if (table[insertIndex].status != 0)
    {
        return solveInsertCollision(key);
    }
    else
    {
        key.status = 1;
        table[insertIndex] = key;
        return 0; // Ключ вставлен "0"
    }
}

bool table2::HashTable::solveDeleteCollision(HashTableEntry key)
{
    int i;
    int j = 0;
    do
    {
        j++;
        i = secondHash(key.firstHash, 1, 1, j);

        if (table[i] == key && table[i].status == 1)
            table[i].status = 2;
    }
    while (j < size && (table[i] != key || table[i].status != 1) && table[i].status != 0);

    if (table[i].status == 2)
        return true;
    else return false;
}

bool table2::HashTable::remove(HashTableEntry key)
{
    int i = firstHash(key.key, size);
    if (table[i].status == 1)
    {
        if (table[i] == key)
        {
            table[i].status = 2;
            return true;
        }
        else
            return solveDeleteCollision(key);
    }
    else if (table[i].status == 2)
        return solveDeleteCollision(key);
}

table2::HashTableEntry *table2::HashTable::find(long long key)
{
    int i = firstHash(key, size);
    int firstHash = i;
    if (table[i].status == 1 && table[i].key == key)
        return &table[i];
    else
    {
        int j = 1;
        while (j <= size)
        {
            i = secondHash(firstHash, 1, 1, j);
            if (table[i].key == key && table[i].status == 1)
                return &table[i];
            j++;
        }
    }

    return nullptr;
}

void table2::HashTable::printToQTableWidget(QTableWidget *tableWidget)
{
    for(int i = 0; i < size; i++)
    {
        // Строки элементов таблицы
        QString indexString = QString::number(i);
        QString firstHashString = ((table[i].firstHash != -1) ? QString::number(table[i].firstHash) : "");
        QString secondHashString = ((table[i].secondHash != -1) ? QString::number(table[i].secondHash) : "");
        QString keyString = ((table[i].key != -1) ? QString::number(table[i].key) : "");
        QString valueString = ((table[i].value != -1) ? QString::number(table[i].value + 1) : "");
        QString statusString = QString::number(table[i].status);

        // Элементы строки
        QTableWidgetItem *indexItem = new QTableWidgetItem(indexString);
        QTableWidgetItem *firstHashItem = new QTableWidgetItem(firstHashString);
        QTableWidgetItem *secondHashItem = new QTableWidgetItem(secondHashString);
        QTableWidgetItem *keyItem = new QTableWidgetItem(keyString);
        QTableWidgetItem *valueItem = new QTableWidgetItem(valueString);
        QTableWidgetItem *statusItem = new QTableWidgetItem(statusString);

        // Устанавливаем флаг запрета редактирования для каждого элемента
        indexItem->setFlags(indexItem->flags() & ~Qt::ItemIsEditable);
        firstHashItem->setFlags(firstHashItem->flags() & ~Qt::ItemIsEditable);
        secondHashItem->setFlags(secondHashItem->flags() & ~Qt::ItemIsEditable);
        keyItem->setFlags(keyItem->flags() & ~Qt::ItemIsEditable);
        valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
        statusItem->setFlags(statusItem->flags() & ~Qt::ItemIsEditable);

        // Заносим строку в таблицу
        int rowIndex = tableWidget->rowCount();
        tableWidget->insertRow(rowIndex);
        tableWidget->setItem(rowIndex, 0, indexItem);
        tableWidget->setItem(rowIndex, 1, firstHashItem);
        tableWidget->setItem(rowIndex, 2, secondHashItem);
        tableWidget->setItem(rowIndex, 3, keyItem);
        tableWidget->setItem(rowIndex, 4, valueItem);
        tableWidget->setItem(rowIndex, 5, statusItem);
    }
}

table2::HashTable::~HashTable()
{
    delete[] table;
}

void table2::HashTable::clear()
{
    for(int i = 0; i < size; i++)
        table[i] = HashTableEntry();
}

void table2::SingleLinkedList::clear()
{
    SingleLinkedListNode *currentNode = head;
    while (head != nullptr)
    {
        head = currentNode->next;
        delete currentNode;
        currentNode = head;
    }
}

void table2::SingleLinkedList::insertNode(int value)
{
    SingleLinkedListNode *currentNode = head;
    if (head == nullptr)
    {
        head = new SingleLinkedListNode;
        head->value = value;
        head->next = nullptr;
    }
    else
    {
        while (currentNode->next != nullptr)
            currentNode = currentNode->next;
        currentNode->next = new SingleLinkedListNode;
        currentNode = currentNode->next;
        currentNode->value = value;
        currentNode->next = nullptr;
    }
}

int table2::SingleLinkedList::countNodes()
{
    int count = 0;
    SingleLinkedListNode *currentNode = head;
    while (currentNode != nullptr)
    {
        count++;
        currentNode = currentNode->next;
    }
    return count;
}

void table2::SingleLinkedList::removeNode(int value)
{
    SingleLinkedListNode *currentNode = head;
    if (head != nullptr)
    {
        while (currentNode->next != nullptr && currentNode->next->value != value)
            currentNode = currentNode->next;
        if (currentNode->next != nullptr)
        {
            SingleLinkedListNode *temp = currentNode->next->next;
            delete currentNode->next;
            currentNode->next = temp;
        }
    }
}

QString table2::SingleLinkedList::getPrintableString()
{
    QString str;
    auto *curr = head;
    while (curr != nullptr)
    {
        str += QString::number(curr->value + 1);
        curr = curr->next;
        if (curr != nullptr)
            str += " ";
    }
    return str;
}

table2::SingleLinkedListNode *table2::SingleLinkedList::getHead()
{
    return head;
}
