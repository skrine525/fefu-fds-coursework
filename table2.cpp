#include "table2.h"

#include <QString>

table2::Address::operator QString() const
{
    return QString("%1, %2").arg(region, district);
}

bool table2::operator==(Key &a, Key &b)
{
    return a.phoneNumber == b.phoneNumber;
}

bool table2::operator!=(Key &a, Key &b)
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

table2::Key::Key(long long phoneNumber)
{
    this->phoneNumber = phoneNumber;
}

table2::Address::Address(QString region, QString district)
{
    this->region = region;
    this->district = district;
}

int table2::HashTable::firstHash(long long phoneNumber, int N)
{
    QString str = QString::number(phoneNumber);
    long long hash = 0;

    for (int i = 0; i < 11; i += 2)
    {
        hash += str.mid(i, 2).toInt();
    }

    return hash % N;
}

int table2::HashTable::secondHash(int firstHash, int k1, int k2, int j)
{
    return (firstHash + j  *k1 + j  *j  *k2) % N;
}

table2::HashTable::HashTable(int maxN)
{
    N = maxN;
    Keys = new Key[N];
    for (int i = 0; i < N; i++)
    {
        Keys[i] = Key();
    }
}

bool table2::HashTable::solveInsertCollision(Key key)
{
    int insertIndex = firstHash(key.phoneNumber, N);
          if (Keys[insertIndex].status == 2)
          {
              bool canInsert = true;

              int i;
              int j = 0;
              do
              {
                  j++;
                  i = secondHash(key.firstHash, 1, 1, j);

                  if (Keys[i] == key && Keys[i].status == 1)
                      canInsert = false;

              } while (j < N && (Keys[i] != key || Keys[i].status != 1) && Keys[i].status != 0);

              if (canInsert)
              {
                  key.status = 1;
                  Keys[insertIndex] = key;
              }
          }
          else if (Keys[insertIndex] != key)
          {
              insertIndex = -1;

              int i;
              int j = 0;
              do
              {
                  j++;
                  i = secondHash(key.firstHash, 1, 1, j);

                  if (insertIndex == -1 && (Keys[i].status == 2 || Keys[i].status == 0))
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
                  return true;
              }
              else
              {
                    return false;
              }
          }
}

bool table2::HashTable::insert(Key key)
{
    int insertIndex = firstHash(key.phoneNumber, N);
    key.firstHash = insertIndex;
    if (Keys[insertIndex].status != 0)
    {
        return solveInsertCollision(key);
    }
    else
    {
        key.status = 1;
        Keys[insertIndex] = key;
        return true;
    }
}

bool table2::HashTable::solveDeleteCollision(Key key)
{
    int i;
    int j = 0;
    do
    {
        j++;
        i = secondHash(key.firstHash, 1, 1, j);

        if (Keys[i] == key && Keys[i].status == 1)
            Keys[i].status = 2;
    }
    while (j < N && (Keys[i] != key || Keys[i].status != 1) && Keys[i].status != 0);

    if (Keys[i].status == 2)
        return true;
    else return false;
}

bool table2::HashTable::remove(Key key)
{
    int i = firstHash(key.phoneNumber, N);
            if (Keys[i].status == 1)
            {
                if (Keys[i] == key)
                {
                    Keys[i].status = 2;
                    return true;
                }
                else
                    return solveDeleteCollision(key);
            }
            else if (Keys[i].status == 2)
                return solveDeleteCollision(key);
}

int table2::HashTable::search(long long phoneNumber)
{
    int i = firstHash(phoneNumber, N);
    int firstHash = i;
    if (Keys[i].status == 1 && Keys[i].phoneNumber == phoneNumber)
        return i;
    else
    {
        int j = 1;
        while (j <= N)
        {
            i = secondHash(firstHash, 1, 1, j);
            if (Keys[i].phoneNumber == phoneNumber && Keys[i].status == 1)
                return i;
            j++;
        }
    }

    return -1;
}

void table2::HashTable::print()
{

}

table2::HashTable::~HashTable()
{
    delete[] Keys;
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

void table2::SingleLinkedList::deleteNode(int value)
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
