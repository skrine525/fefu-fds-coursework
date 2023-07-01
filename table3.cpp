#include "table3.h"

#include <QString>

table3::Datetime::operator QString() const
{
    QString dayString;
    if (this->day < 10)
        dayString = "0" + QString::number(this->day);
    else
        dayString = QString::number(this->day);

    QString monthString;
    if(this->month < 10)
        monthString = "0" + QString::number(this->month);
    else
        monthString = QString::number(this->month);

    QString yearString = QString::number(this->year);

    QString hourString;
    if(this->hour < 10)
        hourString = "0" + QString::number(this->hour);
    else
        hourString = QString::number(this->hour);

    QString minuteString;
    if(this->minute < 10)
        minuteString = "0" + QString::number(this->minute);
    else
        minuteString = QString::number(this->minute);

    return QString("%1.%2.%3 %4:%5").arg(dayString, monthString, yearString,
                                         hourString, minuteString);
}

bool table3::operator<(const table3::Datetime& a, const table3::Datetime& b)
{
    if (a.year != b.year)
        return a.year < b.year;
    if (a.month != b.month)
        return a.month < b.month;
    if (a.day != b.day)
        return a.day < b.day;
    if (a.hour != b.hour)
        return a.hour < b.hour;
    return a.minute < b.minute;
}

bool table3::operator>(const table3::Datetime& a, const table3::Datetime& b)
{
    return b < a;
}

bool table3::operator==(const table3::Datetime& a, const table3::Datetime& b)
{
    return a.year == b.year && a.month == b.month && a.day == b.day &&
            a.hour == b.hour && a.minute == b.minute;
}

bool table3::operator!=(const table3::Datetime& a, const table3::Datetime& b)
{
    return !(a == b);
}

table3::DoublyLinkedRingList::~DoublyLinkedRingList()
{
    this->clear();
}

table3::DoublyLinkedRingListNode *table3::DoublyLinkedRingList::getHead()
{
    return head;
}

void table3::DoublyLinkedRingList::clear()
{
    table3::DoublyLinkedRingListNode *curr = head;
    if (curr != nullptr)
    {
        do
        {
            table3::DoublyLinkedRingListNode *d = curr;
            curr = curr->next;
            delete d;
        } while (curr != head);
    }
    head = nullptr;
}

bool table3::DoublyLinkedRingList::isEmpty()
{
    return (head == nullptr);
}

QString table3::DoublyLinkedRingList::getPrintableString()
{
    QString str;
    table3::DoublyLinkedRingListNode *curr = head;

    if (curr != nullptr)
    {
        do
        {
            str += QVariant(curr->value + 1).toString();
            curr = curr->next;
            if (curr != head)
                str += " ";
        } while (curr != head);
    }
    return str;
}

void table3::DoublyLinkedRingList::deleteNode(int value)
{
    table3::DoublyLinkedRingListNode *curr = head;
    if (curr != nullptr)
    {
        do
        {
            if (curr->value == value)
            {
                if (curr == head && curr->next == head)
                {
                    delete curr;
                    head = nullptr;
                }
                else
                {
                    table3::DoublyLinkedRingListNode *prev = curr->prev;
                    table3::DoublyLinkedRingListNode *next = curr->next;
                    prev->next = next;
                    next->prev = prev;
                    if (curr == head)
                        head = curr->next;
                    delete curr;
                    curr = nullptr;
                }
            }
            else
                curr = curr->next;
        } while (curr != head && curr != nullptr && head != nullptr);
    }
}

void table3::DoublyLinkedRingList::insertNode(int value)
{
    if (head == nullptr)
    {
        head = new table3::DoublyLinkedRingListNode;
        head->next = head;
        head->prev = head;
        head->value = value;
    }
    else
    {
        table3::DoublyLinkedRingListNode *curr = head;
        do
        {
            if (curr->value != value && value < curr->value)
            {
                table3::DoublyLinkedRingListNode *ncurr
                        = new table3::DoublyLinkedRingListNode;
                ncurr->value = value;
                if (curr == head)
                    head = ncurr;

                ncurr->next = curr;
                ncurr->prev = curr->prev;
                curr->prev->next = ncurr;
                curr->prev = ncurr;
            }
            else if (curr->value != value && curr->next == head)
            {
                table3::DoublyLinkedRingListNode *ncurr
                        = new table3::DoublyLinkedRingListNode;
                ncurr->value = value;
                ncurr->next = curr->next;
                ncurr->prev = curr;
                curr->next->prev = ncurr;
                curr->next = ncurr;
            }
            else
                curr = curr->next;
        } while (curr->prev->value < value);
    }
}
