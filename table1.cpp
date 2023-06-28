#include "table1.h"

#include <QString>

void table1::DoubleLinkedList::clear()
{
    DoubleLinkedListNode* currentNode = head;

    if (currentNode != nullptr) {
        do {
            DoubleLinkedListNode* d = currentNode;
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
        DoubleLinkedListNode* currentNode = head;

        do {
            if (currentNode->value != value && value < currentNode->value)
            {

                DoubleLinkedListNode* n = new DoubleLinkedListNode;
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
                DoubleLinkedListNode* n = new DoubleLinkedListNode;
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
    DoubleLinkedListNode* currentNode = head;
    while (currentNode != nullptr)
    {
        count++;
        currentNode = currentNode->next;
    }
    return count;
}

void table1::DoubleLinkedList::deleteNode(int value)
{
    DoubleLinkedListNode* currentNode = head;

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
