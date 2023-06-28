#include "table2.h"

#include <QString>

bool table2::operator<(QString& a, QString& b)
{
    if (a.compare(b) < 0) return 1;
    else return 0;
}

bool table2::operator>(QString& a, QString& b)
{
    if (a.compare(b) > 0) return 1;
    else return 0;
}

bool table2::operator==(QString& a, QString& b)
{
    if (a.compare(b) == 0) return 1;
    else return 0;
}

bool table2::operator!=(QString& a, QString& b)
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

void table2::SingleLinkedList::clear()
{
    SingleLinkedListNode* currentNode = head;
    while (head != nullptr) {
        head = currentNode->next;
        delete currentNode;
        currentNode = head;
    }
}

void table2::SingleLinkedList::insertNode(int value)
{
    SingleLinkedListNode* currentNode = head;
    if (head == nullptr) {
        head = new SingleLinkedListNode;
        head->value = value;
        head->next = nullptr;
    }
    else {
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
    SingleLinkedListNode* currentNode = head;
    while (currentNode != nullptr)
    {
        count++;
        currentNode = currentNode->next;
    }
    return count;
}

void table2::SingleLinkedList::deleteNode(int value)
{
    SingleLinkedListNode* currentNode = head;
    if (head != nullptr)
    {
        while (currentNode->next != nullptr && currentNode->next->value != value)
            currentNode = currentNode->next;
        if (currentNode->next != nullptr)
        {
            SingleLinkedListNode* temp = currentNode->next->next;
            delete currentNode->next;
            currentNode->next = temp;
        }
    }
}
