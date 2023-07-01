#include "table1.h"
#include <QString>

bool table1::operator==(Key& a, Key& b)
{
    return a.phoneNumber == b.phoneNumber;
}

bool table1::operator!=(Key& a, Key& b)
{
    return !(a == b);
}

table1::Key::Key(long long phoneNumber)
{
    this->phoneNumber = phoneNumber;
}

int table1::HashTable::firstHash(long long phoneNumber, int N)
{
    return phoneNumber % N;
}

int table1::HashTable::secondHash(int firstHash, int k, int j)
{
    return (firstHash + k * j) % N;
}

table1::HashTable::HashTable(int maxN)
{
    N = maxN;
    Keys = new Key[N];
    for (int i = 0; i < N; i++)
    {
        Keys[i] = Key();
    }
}

bool table1::HashTable::solveInsertCollision(Key key)
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
                  i = secondHash(key.firstHash, 1, j);

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
                  i = secondHash(key.firstHash, 1, j);

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

bool table1::HashTable::insert(Key key)
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

bool table1::HashTable::solveDeleteCollision(Key key)
{
    int i;
    int j = 0;
    do
    {
        j++;
        i = secondHash(key.firstHash, 1, j);

        if (Keys[i] == key && Keys[i].status == 1)
            Keys[i].status = 2;

    } while (j < N && (Keys[i] != key || Keys[i].status != 1) && Keys[i].status != 0);
    if (Keys[i].status == 2)
        return true;
    else return false;
}

bool table1::HashTable::remove(Key key)
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

int table1::HashTable::search(long long phoneNumber)
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
            i = secondHash(firstHash, 1, j);
            if (Keys[i].phoneNumber == phoneNumber && Keys[i].status == 1)
                return i;
            j++;
        }
    }

    return -1;
}

void table1::HashTable::print()
{

}

table1::HashTable::~HashTable()
{
    delete[] Keys;
}

void table1::DoubleLinkedList::clear()
{
    DoubleLinkedListNode *currentNode = head;

    if (currentNode != nullptr) {
        do {
            DoubleLinkedListNode *d = currentNode;
            currentNode = currentNode->next;
            delete d;
        } while (currentNode != nullptr);
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

        do {
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
        } while (currentNode != nullptr && ((currentNode != head && value > currentNode->prev->value) || (currentNode == head && value > currentNode->value)));
    }
}

int table1::DoubleLinkedList::countNodes()
{
    int count = 0;
    DoubleLinkedListNode *currentNode = head;
    while (currentNode != nullptr)
    {
        count++;
        currentNode = currentNode->next;
    }
    return count;
}

void table1::DoubleLinkedList::deleteNode(int value)
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
        } while (currentNode != nullptr && head != nullptr);
    }
}
