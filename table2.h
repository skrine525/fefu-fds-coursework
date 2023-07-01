#ifndef TABLE2_H
#define TABLE2_H

#include <QString>
#include <QVector>
#include <QVariant>

namespace table2
{
    
    struct Record
    {
        long long phoneNumber;
        QString fullName;
        QString region;
        QString district;
        unsigned age;
    };

    struct HashTableEntry
    {
        long long key = 0;
        int value = 0;
        int firstHash = 0;
        int status = 0;

        HashTableEntry() { }

        HashTableEntry(long long key, int value);
    };

    bool operator==(HashTableEntry &a, HashTableEntry &b);
    bool operator!=(HashTableEntry &a, HashTableEntry &b);

    class HashTable
    {
    private:
        int size;
        HashTableEntry *table;
        int firstHash(long long key, int size);
        int secondHash(int firstHash, int k1, int k2, int j);
        bool solveInsertCollision(HashTableEntry key);
        bool solveDeleteCollision(HashTableEntry key);

    public:
        HashTable(int maxN);
        bool insert(HashTableEntry key);
        bool remove(HashTableEntry key);
        int search(long long key);
        void print();
        ~HashTable();
    };

    struct Address
    {
        QString region;
        QString district;

        Address(QString region, QString district);
        operator QString() const;

    };
    bool operator<(Address &a, Address &b);
    bool operator>(Address &a, Address &b);
    bool operator==(Address &a, Address &b);
    bool operator!=(Address &a, Address &b);

    struct SingleLinkedListNode
    {
        SingleLinkedListNode *next;
        int value;
    };

    class SingleLinkedList
    {
    public:
        SingleLinkedList() : head(nullptr) {}
        void insertNode(int value);
        void deleteNode(int value);
        int countNodes();
        void clear();
        QString getPrintableString();

        SingleLinkedListNode *head;
    };

    template <typename HashTableEntry>
    struct AVLTreeNode
    {
        AVLTreeNode *childR;
        AVLTreeNode *childL;
        HashTableEntry key;
        SingleLinkedList *valueList;
        int balance;
        AVLTreeNode(HashTableEntry k, SingleLinkedList *v) : key(k), valueList(v), childL(nullptr), childR(nullptr), balance(0) {}
    };

    template <typename HashTableEntry>
    class AVLTree
    {
    public:
        AVLTree() : root(nullptr) {}
        void insertNode(HashTableEntry key, int value);
        void deleteNode(HashTableEntry key, int value);
        AVLTreeNode<HashTableEntry> *findNode(HashTableEntry key);
        QString getPrintableHtml(int l) const;
        void clear();

    private:
        AVLTreeNode<HashTableEntry> *root;
        void print(AVLTreeNode<HashTableEntry> *&node, unsigned h);
        void insertNode(HashTableEntry key, int value, bool &h, AVLTreeNode<HashTableEntry> *&currentNode);
        void clear(AVLTreeNode<HashTableEntry> *&node);
        void deleteNode(HashTableEntry key, int value, bool &h, AVLTreeNode<HashTableEntry> *&currentNode);
        void deleteExchange(AVLTreeNode<HashTableEntry> *&currentNode, AVLTreeNode<HashTableEntry> *&nq, bool &h);
        void balanceR(AVLTreeNode<HashTableEntry> *&currentNode, bool &h);
        void balanceL(AVLTreeNode<HashTableEntry> *&currentNode, bool &h);
        QString getPrintableHtml(AVLTreeNode<HashTableEntry> *node, int h, int l) const;
    };

    struct Patients
    {
        QVector<Record> records;
        AVLTree<Address> addressTree;
        AVLTree<QString> fullNameTree;
        AVLTree<unsigned> ageTree;
    };
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::clear(AVLTreeNode<HashTableEntry> *&node)
{
    if (node != nullptr)
    {
        clear(node->childL);
        clear(node->childR);
        node->valueList->clear();
        delete node;
    }
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::clear()
{
    if (root != nullptr)
    {
        clear(root->childL);
        clear(root->childR);
        root->valueList->clear();
        delete root;
        root = nullptr;
    }
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::insertNode(HashTableEntry key, int value, bool &h, AVLTreeNode<HashTableEntry> *&currentNode)
{
    if (currentNode == nullptr)
    {
        SingleLinkedList *valueList = new SingleLinkedList;
        valueList->insertNode(value);
        currentNode = new AVLTreeNode<HashTableEntry>(key, valueList);
        h = true;
        currentNode->balance = 0;
    }
    else if (currentNode->key > key)
    {
        insertNode(key, value, h, currentNode->childL);
        if (h)
        {
            if (currentNode->balance == 1)
            {
                currentNode->balance = 0;
                h = false;
            }
            else if (currentNode->balance == 0)
                currentNode->balance = -1;
            else
            {
                AVLTreeNode<HashTableEntry> *node1 = currentNode->childL;
                if (node1->balance == -1)
                {
                    currentNode->childL = node1->childR;
                    node1->childR = currentNode;
                    currentNode->balance = 0;
                    currentNode = node1;
                }
                else
                {
                    AVLTreeNode<HashTableEntry> *node2 = node1->childR;
                    node1->childR = node2->childL;
                    node2->childL = node1;
                    currentNode->childL = node2->childR;
                    node2->childR = currentNode;
                    if (node2->balance == -1)
                        currentNode->balance = 1;
                    else currentNode->balance = 0;
                    if (node2->balance == 1)
                        node1->balance = -1;
                    else node1->balance = 0;
                    currentNode = node2;
                }
                currentNode->balance = 0;
                h = false;
            }
        }
    }
    else if (currentNode->key < key)
    {
        insertNode(key, value, h, currentNode->childR);
        if (h)
        {
            if (currentNode->balance == -1)
            {
                currentNode->balance = 0;
                h = false;
            }
            else if (currentNode->balance == 0)
                currentNode->balance = 1;
            else
            {
                AVLTreeNode<HashTableEntry> *node1 = currentNode->childR;
                if (node1->balance == 1)
                {
                    currentNode->childR = node1->childL;
                    node1->childL = currentNode;
                    currentNode->balance = 0;
                    currentNode = node1;
                }
                else
                {
                    AVLTreeNode<HashTableEntry> *node2 = node1->childL;
                    node1->childL = node2->childR;
                    node2->childR = node1;
                    currentNode->childR = node2->childL;
                    node2->childL = currentNode;
                    if (node2->balance == 1)
                        currentNode->balance = -1;
                    else currentNode->balance = 0;
                    if (node2->balance == -1)
                        node1->balance = 1;
                    else node1->balance = 0;
                    currentNode = node2;
                }
                currentNode->balance = 0;
                h = false;
            }
        }
    }
    else
    {
        currentNode->valueList->insertNode(value);
    }
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::insertNode(HashTableEntry key, int value)
{
    bool height = false;
    insertNode(key, value, height, root);
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::balanceR(AVLTreeNode<HashTableEntry> *&currentNode, bool &h)
{
    if (currentNode->balance == 1)
        currentNode->balance = 0;
    else if (currentNode->balance == 0)
    {
        currentNode->balance = -1;
        h = false;
    }
    else
    {
        AVLTreeNode<HashTableEntry> *node1 = currentNode->childL;
        if (node1->balance <= 0)
        {
            currentNode->childL = node1->childR;
            node1->childR = currentNode;
            if (node1->balance == 0)
            {
                currentNode->balance = -1;
                node1->balance = 1;
                h = false;
            }
            else
            {
                currentNode->balance = 0;
                node1->balance = 0;
            }
            currentNode = node1;
        }
        else
        {
            AVLTreeNode<HashTableEntry> *node2 = node1->childR;
            node1->childR = node2->childL;
            node2->childL = node1;
            currentNode->childL = node2->childR;
            node2->childR = currentNode;
            if (node2->balance == -1)
                currentNode->balance = 1;
            else currentNode->balance = 0;
            if (node2->balance == 1)
                node1->balance = -1;
            else node1->balance = 0;
            node2->balance = 0;
            currentNode = node2;
        }
    }
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::balanceL(AVLTreeNode<HashTableEntry> *&currentNode, bool &h)
{
    if (currentNode->balance == -1)
        currentNode->balance = 0;
    else if (currentNode->balance == 0)
    {
        currentNode->balance = 1;
        h = false;
    }
    else
    {
        AVLTreeNode<HashTableEntry> *node1 = currentNode->childR;
        if (node1->balance >= 0)
        {
            currentNode->childR = node1->childL;
            node1->childL = currentNode;
            if (node1->balance == 0)
            {
                currentNode->balance = 1;
                node1->balance = -1;
                h = false;
            }
            else
            {
                currentNode->balance = 0;
                node1->balance = 0;
            }
            currentNode = node1;
        }
        else
        {
            AVLTreeNode<HashTableEntry> *node2 = node1->childL;
            node1->childL = node2->childR;
            node2->childR = node1;
            currentNode->childR = node2->childL;
            node2->childL = currentNode;
            if (node2->balance == 1)
                currentNode->balance = -1;
            else currentNode->balance = 0;
            if (node2->balance == -1)
                node1->balance = 1;
            else node1->balance = 0;
            currentNode = node2;
            node2->balance = 0;
        }
    }
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::deleteExchange(AVLTreeNode<HashTableEntry> *&currentNode, AVLTreeNode<HashTableEntry> *&nq, bool &h)
{
    if (currentNode->childR != nullptr)
    {
        deleteExchange(currentNode->childR, nq, h);
        if (h) balanceR(currentNode, h);
    }
    else
    {
        nq->key = currentNode->key;
        nq = currentNode;
        currentNode = currentNode->childL;
        h = true;
    }
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::deleteNode(HashTableEntry key, int value, bool &h, AVLTreeNode<HashTableEntry> *&currentNode)
{
    if (currentNode != nullptr)
    {
        if (currentNode->key > key)
        {
            deleteNode(key, value, h, currentNode->childL);
            if (h) balanceL(currentNode, h);
        }
        else if (currentNode->key < key)
        {
            deleteNode(key, value, h, currentNode->childR);
            if (h) balanceR(currentNode, h);
        }
        else
        {
            if (currentNode->valueList->head->next != nullptr)
            {
                currentNode->valueList->deleteNode(value);
            }
            else
            {
                AVLTreeNode<HashTableEntry> *nq = currentNode;
                if (nq->childR == nullptr)
                {
                    currentNode = nq->childL;
                    h = true;
                }
                else if (nq->childL == nullptr)
                {
                    currentNode = nq->childR;
                    h = true;
                }
                else
                {
                    deleteExchange(nq->childL, nq, h);
                    if (h) balanceL(currentNode, h);
                }
                delete nq;
            }
        }
    }
}

template <typename HashTableEntry>
void table2::AVLTree<HashTableEntry>::deleteNode(HashTableEntry key, int value)
{
    bool height = false;
    deleteNode(key, value, height, root);
}


template <typename HashTableEntry>
table2::AVLTreeNode<HashTableEntry> *table2::AVLTree<HashTableEntry>::findNode(HashTableEntry key)
{
    table2::AVLTreeNode<HashTableEntry> *curr = root;
    table2::AVLTreeNode<HashTableEntry> *desired = nullptr;
    if (curr != nullptr)
    {
        do
        {
            if (key < curr->key)
                curr = curr->left;
            else if (key > curr->key)
                curr = curr->right;
            else
                desired = curr;
        } while (curr != nullptr && desired == nullptr);
    }

    return desired;
}

template <typename HashTableEntry>
QString table2::AVLTree<HashTableEntry>::getPrintableHtml(int l) const
{
    QString outputStr = getPrintableHtml(root, 0, l);
    return outputStr + "\n";
}

template <typename HashTableEntry>
QString table2::AVLTree<HashTableEntry>::getPrintableHtml(AVLTreeNode<HashTableEntry> *node,
                                                     int h, int l) const
{
    if (node != nullptr)
    {
        QString outputStr = getPrintableHtml(node->childR, h + l, l);
        QString spaceSymbol(" ");
        QString listPrintableString = node->valueList->getPrintableString();
        outputStr+= "<p style=\"white-space:pre\">" +
                spaceSymbol.repeated(h) + QVariant(node->key).toString() +
                " [" + listPrintableString + "]</p>";
        outputStr += getPrintableHtml(node->childL, h + l, l);
        return outputStr;
    }
    return "";
}
#endif
