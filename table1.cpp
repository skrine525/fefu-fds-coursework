#include "table1.h"
#include <QString>

bool table1::operator==(HashTableEntry &a, HashTableEntry &b)
{
    return a.key == b.key;
}

bool table1::operator!=(HashTableEntry &a, HashTableEntry &b)
{
    return !(a == b);
}

table1::HashTableEntry::HashTableEntry(long long key, int value)
{
    this->key = key;
    this->value = value;
}

int table1::HashTable::firstHash(long long key, int size)
{
    return key % size;
}

int table1::HashTable::secondHash(int firstHash, int k, int j)
{
    return (firstHash + k * j) % size;
}

table1::HashTable::HashTable(int maxN)
{
    size = maxN;
    table = new HashTableEntry[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = HashTableEntry();
    }
}

int table1::HashTable::solveInsertCollision(HashTableEntry key)
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
            i = secondHash(key.firstHash, 1, j);

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
            i = secondHash(key.firstHash, 1, j);

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
            key.secondHash = i;
            key.status = 1;
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


int table1::HashTable::insert(HashTableEntry key)
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

bool table1::HashTable::solveDeleteCollision(HashTableEntry key)
{
    int i;
    int j = 0;
    do
    {
        j++;
        i = secondHash(key.firstHash, 1, j);

        if (table[i] == key && table[i].status == 1)
            table[i].status = 2;

    } while (j < size && (table[i] != key || table[i].status != 1) && table[i].status != 0);
    if (table[i].status == 2)
        return true;
    else return false;
}

bool table1::HashTable::remove(HashTableEntry key)
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

table1::HashTableEntry *table1::HashTable::find(long long key)
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
            i = secondHash(firstHash, 1, j);
            if (table[i].key == key && table[i].status == 1)
                return &table[i];
            j++;
        }
    }

    return nullptr;
}

void table1::HashTable::printToQTableWidget(QTableWidget *tableWidget)
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

table1::HashTable::~HashTable()
{
    delete[] table;
}

void table1::HashTable::clear()
{
    for(int i = 0; i < size; i++)
        table[i] = HashTableEntry();
}

void table1::DoubleLinkedList::clear()
{
    DoubleLinkedListNode *currentNode = head;

    if (currentNode != nullptr) {
        do {
            DoubleLinkedListNode *d = currentNode;
            currentNode = currentNode->next;
            delete d;
        }
        while (currentNode != nullptr);
        head = nullptr;
        tail = nullptr;
    }
}

void table1::DoubleLinkedList::insertNode(int value)
{
    if (head == nullptr)
    {
        head = new DoubleLinkedListNode;
        head->value = value;
        tail = head;
        head->next = nullptr;
        head->prev = nullptr;
    }
    else
    {
        DoubleLinkedListNode *currentNode = head;

        do
        {
            if (currentNode->value != value && value < currentNode->value)
            {

                DoubleLinkedListNode *n = new DoubleLinkedListNode;
                n->value = value;
                n->prev = currentNode->prev;
                n->next = currentNode;
                if (currentNode == head)
                    head = n;
                else
                    currentNode->prev->next = n;
                currentNode->prev = n;
            }
            else if (currentNode->value != value && currentNode == tail)
            {
                DoubleLinkedListNode *n = new DoubleLinkedListNode;
                n->value = value;
                n->prev = currentNode;
                n->next = nullptr;
                currentNode->next = n;
                tail = n;
            }
            else
                currentNode = currentNode->next;
        }
        while (currentNode != nullptr && ((currentNode != head && value > currentNode->prev->value) || (currentNode == head && value > currentNode->value)));
    }
}

void table1::DoubleLinkedList::removeNode(int value)
{
    DoubleLinkedListNode *currentNode = head;

    if (currentNode != nullptr) {
        do {
            if (currentNode->value == value)
            {
                if (currentNode == head && currentNode == tail)                    
                {
                    delete currentNode;
                    head = nullptr;
                    tail = nullptr;
                }
                else
                {
                    if (currentNode == head)                                
                    {
                        head = currentNode->next;
                        head->prev = nullptr;
                    }
                    else
                        currentNode->prev->next = currentNode->next;

                    if (currentNode == tail)                               
                    {
                        tail = currentNode->prev;
                        tail->next = nullptr;
                    }
                    else
                        currentNode->next->prev = currentNode->prev;

                    delete currentNode;
                    currentNode = nullptr;
                }
            }
            else
                currentNode = currentNode->next;
        }
        while (currentNode != nullptr && head != nullptr);
    }
}

QString table1::DoubleLinkedList::getPrintableString()
{
    QString str;
    auto curr = head;
    if (curr != nullptr)
    {
        do
        {
            str += QString::number(curr->value + 1);
            curr = curr->next;
            if (curr != nullptr)
                str += " ";
        }
        while (curr != nullptr);
    }
    return str;
}

table1::DoubleLinkedListNode *table1::DoubleLinkedList::getHead()
{
    return head;
}
