#ifndef TABLE1_H
#define TABLE1_H

#include <QString>
#include <QVector>
#include <QVariant>
#include <QTableWidget>

// Константа размерности статической хеш-таблицы
#define HASHTABLE_SIZE 100

namespace table1
{
    struct Record
    {
        long long phoneNumber;
        QString fullName;
        unsigned experience;
        QString speciality;
    };

    struct HashTableEntry
    {
        long long key = -1;
        int value = -1;
        int firstHash = -1;
        int secondHash = -1; // Изменено на -1 так как 0 всё-таки является индексом
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
        HashTableEntry* table;
        int firstHash(long long key, int size);
        int secondHash(int firstHash, int k, int j);
        int solveInsertCollision(HashTableEntry key);
        bool solveDeleteCollision(HashTableEntry key);

    public:
        HashTable(int maxN);
        int insert(HashTableEntry key);
        bool remove(HashTableEntry key);
        HashTableEntry *find(long long key);
        void printToQTableWidget(QTableWidget *tableWidget);
        void clear();
        ~HashTable();
    };

    struct DoubleLinkedListNode
    {
        DoubleLinkedListNode *next;
        DoubleLinkedListNode *prev;
        int value;
    };

    class DoubleLinkedList
    {
    public:
        DoubleLinkedList() : head(nullptr), tail(nullptr) {}
        void insertNode(int value);
        void removeNode(int value);
        int countNodes();
        void clear();
        QString getPrintableString();
        DoubleLinkedListNode *getHead();

        DoubleLinkedListNode *head;
        DoubleLinkedListNode *tail;
    };

    template <typename Key>
    struct AVLTreeNode
    {
        AVLTreeNode *right;
        AVLTreeNode *left;
        Key key;
        DoubleLinkedList *valueList;
        int balance;
        AVLTreeNode(Key k, DoubleLinkedList *v) : key(k), valueList(v), left(nullptr), right(nullptr), balance(0) {}
    };

    template <typename Key>
    class AVLTree
    {
    public:
        AVLTree() : root(nullptr) {}
        void insertNode(Key key, int value);
        void removeNode(Key key, int value);
        AVLTreeNode<Key> *findNode(Key key);
        QString getPrintableHtml(int l) const;
        void clear();

    private:
        AVLTreeNode<Key> *root;
        void print(AVLTreeNode<Key> *&node, unsigned h);
        void insertNode(Key key, int value, bool &h, AVLTreeNode<Key> *&currentNode);
        void clear(AVLTreeNode<Key> *&node);
        void removeNode(Key key, int value, bool &h, AVLTreeNode<Key> *&currentNode);
        void replaceMinR(AVLTreeNode<Key> *&currentNode, AVLTreeNode<Key> *&nq, bool &h);
        void balanceR(AVLTreeNode<Key> *&currentNode, bool &h);
        void balanceL(AVLTreeNode<Key> *&currentNode, bool &h);
        QString getPrintableHtml(AVLTreeNode<Key> *node, int h, int l) const;
    };

    struct Doctors
    {
        QVector<Record> records;
        AVLTree<QString> specialityTree;
        AVLTree<QString> fullNameTree;
        AVLTree<unsigned> experienceTree;
        HashTable phoneNumberHashTable;

        Doctors() : phoneNumberHashTable(HASHTABLE_SIZE) {}
    };
}

template <typename Key>
void table1::AVLTree<Key>::clear(AVLTreeNode<Key> *&node)
{
    if (node != nullptr)
    {
        clear(node->left);
        clear(node->right);
        node->valueList->clear();
        delete node;
    }
}

template <typename Key>
void table1::AVLTree<Key>::clear()
{
    if (root != nullptr)
    {
        clear(root->left);
        clear(root->right);
        root->valueList->clear();
        delete root;
        root = nullptr;
    }
}

template <typename Key>
void table1::AVLTree<Key>::insertNode(Key key, int value, bool &h, AVLTreeNode<Key> *&currentNode)
{
    if (currentNode == nullptr)
    {
        DoubleLinkedList *valueList = new DoubleLinkedList;
        valueList->insertNode(value);
        currentNode = new AVLTreeNode<Key>(key, valueList);
        h = true;
        currentNode->balance = 0;
    }
    else if (currentNode->key > key)
    {
        insertNode(key, value, h, currentNode->left);
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
                AVLTreeNode<Key> *node1 = currentNode->left;
                if (node1->balance == -1)
                {
                    currentNode->left = node1->right;
                    node1->right = currentNode;
                    currentNode->balance = 0;
                    currentNode = node1;
                }
                else
                {
                    AVLTreeNode<Key> *node2 = node1->right;
                    node1->right = node2->left;
                    node2->left = node1;
                    currentNode->left = node2->right;
                    node2->right = currentNode;
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
        insertNode(key, value, h, currentNode->right);
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
                AVLTreeNode<Key> *node1 = currentNode->right;
                if (node1->balance == 1)
                {
                    currentNode->right = node1->left;
                    node1->left = currentNode;
                    currentNode->balance = 0;
                    currentNode = node1;
                }
                else
                {
                    AVLTreeNode<Key> *node2 = node1->left;
                    node1->left = node2->right;
                    node2->right = node1;
                    currentNode->right = node2->left;
                    node2->left = currentNode;
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

template <typename Key>
void table1::AVLTree<Key>::insertNode(Key key, int value)
{
    bool height = false;
    insertNode(key, value, height, root);
}

template <typename Key>
void table1::AVLTree<Key>::balanceR(AVLTreeNode<Key> *&currentNode, bool &h)
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
        AVLTreeNode<Key> *node1 = currentNode->left;
        if (node1->balance <= 0)
        {
            currentNode->left = node1->right;
            node1->right = currentNode;
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
            AVLTreeNode<Key> *node2 = node1->right;
            node1->right = node2->left;
            node2->left = node1;
            currentNode->left = node2->right;
            node2->right = currentNode;
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


template <typename Key>
void table1::AVLTree<Key>::balanceL(AVLTreeNode<Key> *&currentNode, bool &h)
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
        AVLTreeNode<Key> *node1 = currentNode->right;
        if (node1->balance >= 0)
        {
            currentNode->right = node1->left;
            node1->left = currentNode;
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
            AVLTreeNode<Key> *node2 = node1->left;
            node1->left = node2->right;
            node2->right = node1;
            currentNode->right = node2->left;
            node2->left = currentNode;
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

template <typename Key>
void table1::AVLTree<Key>::replaceMinR(AVLTreeNode<Key> *&currentNode, AVLTreeNode<Key> *&nq, bool &h)
{
    if (nq->left == nullptr)
    {
        currentNode->key = nq->key;
        delete currentNode->valueList;
        currentNode->valueList = nq->valueList;
        currentNode = nq;
        nq = nq->right;
        h = true;
    }
    else
    {
        replaceMinR(currentNode, nq->left, h);
        if (h)
            balanceL(nq, h);
    }
}

template <typename Key>
void table1::AVLTree<Key>::removeNode(Key key, int value, bool &h, AVLTreeNode<Key> *&currentNode)
{
    if (currentNode != nullptr)
    {
        if (currentNode->key > key)
        {
            removeNode(key, value, h, currentNode->left);
            if (h) balanceL(currentNode, h);
        }
        else if (currentNode->key < key)
        {
            removeNode(key, value, h, currentNode->right);
            if (h) balanceR(currentNode, h);
        }
        else
        {
            if (currentNode->valueList->head->next != nullptr)
            {
                currentNode->valueList->removeNode(value);
            }
            else
            {
                AVLTreeNode<Key> *nq = currentNode;
                if (nq->right == nullptr)
                {
                    currentNode = nq->left;
                    h = true;
                }
                else if (nq->left == nullptr)
                {
                    currentNode = nq->right;
                    h = true;
                }
                else
                {
                    replaceMinR(nq, nq->right, h);
                    if (h) balanceR(currentNode, h);
                }
                delete nq;
            }
        }
    }
}

template <typename Key>
void table1::AVLTree<Key>::removeNode(Key key, int value)
{
    bool height = false;
    removeNode(key, value, height, root);
}

template <typename Key>
QString table1::AVLTree<Key>::getPrintableHtml(int l) const
{
    QString outputStr = getPrintableHtml(root, 0, l);
    return outputStr + "\n";
}

template <typename Key>
QString table1::AVLTree<Key>::getPrintableHtml(AVLTreeNode<Key> *node,
                                                     int h, int l) const
{
    if (node != nullptr)
    {
        QString outputStr = getPrintableHtml(node->right, h + l, l);
        QString spaceSymbol(" ");
        QString listPrintableString = node->valueList->getPrintableString();
        outputStr+= "<p style=\"white-space:pre\">" +
                spaceSymbol.repeated(h) + QVariant(node->key).toString() +
                " [" + listPrintableString + "]</p>";
        outputStr += getPrintableHtml(node->left, h + l, l);
        return outputStr;
    }
    return "";
}

template <typename Key>
table1::AVLTreeNode<Key> *table1::AVLTree<Key>::findNode(Key key)
{
    table1::AVLTreeNode<Key> *curr = root;
    table1::AVLTreeNode<Key> *desired = nullptr;
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

#endif
