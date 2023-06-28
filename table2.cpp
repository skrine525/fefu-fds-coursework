#include "table2.h"

#include <QString>

bool table2::operator==(Key& a, Key& b)
{
    return a.phoneNumber == b.phoneNumber;
}

bool table2::operator!=(Key& a, Key& b)
{
    return !(a == b);
}

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

table2::Key(long long phoneNumber)
{
    this->phoneNumber = phoneNumber;
}

int table2::FirstHash(long long phoneNumber, int N)
{
    QString str = QString::number(phoneNumber);
    int size = str.QString::size();
    long long hash = 0;

    for (int i = 0; i < 11; i += 2)
    {
        hash += QString::toInt(str.QString::mid(i, 2));
    }

    return hash % N;
}

int table2::SecondHash(int firstHash, int k1, int k2, int j)
{
    return (firstHash + j * k1 + j * j * k2) % N;
}

table2::HashTable(int maxN)
{
    N = maxN;
    Keys = new Key[N];
    for (int i = 0; i < N; i++)
    {
        Keys[i] = Key();
    }
}

void table2::SolveInsertCollision(Key key)
{
    int insertIndex = 0;
    if (Keys[key.firstHash].status == 2)
        insertIndex = key.firstHash;
    else insertIndex = -1;

    int i = key.firstHash;
    int j = -1;
    do
    {
        j++;
        i = SecondHash(key.firstHash, 1, 1, j);

        if (insertIndex == -1 && Keys[i].status != 1)
        {
            insertIndex = i;
        }
        else if (Keys[i] == key && Keys[i].status == 1)
            insertIndex = -1;

    } while (j < N && (Keys[i] != key || Keys[i].status != 1) && Keys[i].status != 0);

    if (insertIndex != -1)
    {
        key.status = 1;
        Keys[insertIndex] = key;
    }
    else if (j == N)
        std::cout << "Insert place wasn't found\n";
    else return;
}

void table2::Insert(Key key)
{
    int insertIndex = FirstHash(key.phoneNumber, N);
    key.firstHash = insertIndex;
    if (Keys[insertIndex].status != 0)
    {
        SolveInsertCollision(key);
    }
    else
    {
        key.status = 1;
        Keys[insertIndex] = key;
    }
}

void table2::SolveDeleteCollision(Key key)
{
    int i;
    int j = 0;
    do
    {
        j++;
        i = SecondHash(key.firstHash, 1, 1, j);

        if (Keys[i] == key && Keys[i].status == 1)
            Keys[i].status = 2;

    } while (j < N && (Keys[i] != key || Keys[i].status != 1) && Keys[i].status != 0);
}

void table2::Delete(Key key)
{
    int i = FirstHash(key.phoneNumber, N);
    if (Keys[i].status == 1)
        Keys[i].status = 2;
    else if (Keys[i].status == 2)
        SolveDeleteCollision(key);
}

int table2::Search(long long phoneNumber)
{
    int i = FirstHash(phoneNumber, N);
    int firstHash = i;
    if (Keys[i].status == 1 && Keys[i].phoneNumber == phoneNumber)
        return i;
    else
    {
        int j = 1;
        while (j <= N)
        {
            i = SecondHash(firstHash, 1, 1, j);
            if (Keys[i].phoneNumber == phoneNumber && Keys[i].status == 1)
                return i;
            j++;
        }
    }

    return -1;
}

void table2::Print()
{

}

table2::~HashTable()
{
    delete[] Keys;
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
