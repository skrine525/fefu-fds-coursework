#ifndef TABLE3_H
#define TABLE3_H

#include <QString>
#include <QVector>
#include <QVariant>
#include <QtMath>
#include <QTableWidget>
#include <QDebug>

// Константа начальной размерности динамической хеш-таблицы
#define HASHTABLE_SIZE 10

namespace table3
{
    struct Datetime
    {
        unsigned year;
        unsigned month;
        unsigned day;
        unsigned hour;
        unsigned minute;

        operator QString() const;
    };

    bool operator<(const Datetime &a, const Datetime &b);
    bool operator>(const Datetime &a, const Datetime &b);
    bool operator==(const Datetime &a, const Datetime &b);
    bool operator!=(const Datetime &a, const Datetime &b);

    struct Record
    {
        long long doctorPhoneNumber;
        long long patientPhoneNumber;
        Datetime appointmentDatetime;
        unsigned appointmentCost;
    };

    struct PhoneNumberAndDatetime
    {
        long long phoneNumber;
        Datetime datetime;

        PhoneNumberAndDatetime () {}
        PhoneNumberAndDatetime (long long phoneNumber, Datetime datetime);

        operator QString() const;
        operator int() const;
    };

    bool operator==(const PhoneNumberAndDatetime &a, const PhoneNumberAndDatetime &b);
    bool operator!=(const PhoneNumberAndDatetime &a, const PhoneNumberAndDatetime &b);

    template<typename Key>
    class HashTable
    {
    public:
        struct Entry
        {
            Key* key = nullptr;
            int value = -1;
            int status = 0;    // 1: занят, 0: удален
            int hash1 = -1;
            int hash2 = -1;
        };

        HashTable(int initialSize);
        ~HashTable();
        bool insert(Key key, int value);
        Entry *find(Key key);
        bool remove(Key key);
        void printToQTableWidget(QTableWidget *tableWidget);
        void clear();

    private:
        QVector<Entry> table;
        int size;
        int initialSize;
        int primeSize;
        int count;

        bool isPrime(int number);
        void calculatePrimeSize();
        int hash1(Key &key);
        int hash2(int hash1, int key, int j);
        void resize(double factor);
        int findEmptySlot(QVector<Entry> &table, Key key);
    };

    struct DoublyLinkedRingListNode
    {
        DoublyLinkedRingListNode *next;
        DoublyLinkedRingListNode *prev;
        int value;
    };

    class DoublyLinkedRingList
    {
    public:
        DoublyLinkedRingList() : head(nullptr) {}
        ~DoublyLinkedRingList();
        void insertNode(int value);
        void removeNode(int value);
        QString getPrintableString();
        bool isEmpty();
        void clear();
        DoublyLinkedRingListNode *getHead();

    private:
        DoublyLinkedRingListNode *head;
    };

    enum class Color { RED, BLACK };

    template <typename Key>
    struct RBTreeNode
    {
        Key key;
        DoublyLinkedRingList *valueList;
        Color color;
        RBTreeNode *left;
        RBTreeNode *right;
        RBTreeNode *parent;

        explicit RBTreeNode(Key k, DoublyLinkedRingList *v, Color c = Color::RED)
            : key(k), valueList(v), color(c), left(nullptr),
              right(nullptr), parent(nullptr) {}
    };

    template <typename Key>
    class RBTree
    {
    public:
        RBTree() : root(nullptr) {}

        void insertNode(Key key, int value);
        void removeNode(Key key, int value);
        RBTreeNode<Key> *findNode(Key key);
        QString getPrintableHtml(int l) const;
        void clear();

    private:
        RBTreeNode<Key> *root;

        void insertNodeFixup(RBTreeNode<Key> *node);
        void rotateLeft(RBTreeNode<Key> *node);
        void rotateRight(RBTreeNode<Key> *node);
        RBTreeNode<Key> *minimumNode(RBTreeNode<Key> *node);
        void removeNode(RBTreeNode<Key> *node,
                        RBTreeNode<Key> *parent,
                        RBTreeNode<Key> *grandparent, RBTree<Key> &tree);
        void deleteNodeFixup(RBTreeNode<Key> *node,
                             RBTreeNode<Key> *parent,
                             RBTreeNode<Key> *grandparent, RBTree<Key> &tree);
        QString getPrintableHtml(RBTreeNode<Key> *node, int h, int l) const;
        void clear(RBTreeNode<Key> *node);
    };

    struct Appointments
    {
        QVector<Record> records;
        RBTree<long long> doctorPhoneNumberTree;
        RBTree<long long> patientPhoneNumberTree;
        RBTree<Datetime> appointmentDatetimeTree;
        RBTree<unsigned> appointmentCostTree;
        HashTable<PhoneNumberAndDatetime> phoneNumberAndDatetimeHashTable;

        Appointments();
    };
}

template <typename Key>
table3::HashTable<Key>::HashTable(int initialSize) : size(initialSize), initialSize(initialSize), count(0)
{
    table.resize(size);
    calculatePrimeSize();
}

template <typename Key>
table3::HashTable<Key>::~HashTable()
{
    for (size_t i = 0; i < size; i++)
    {
        if (table[i].key)
            delete table[i].key;
    }
}

template <typename Key>
void table3::HashTable<Key>::clear()
{
    for(int i = 0; i < size; i++)
    {
        if(table[i].key)
            delete table[i].key;
    }
    size = initialSize;
    count = 0;
    QVector<Entry> newTable(size);
    table = std::move(newTable);
}

template <typename Key>
void table3::HashTable<Key>::printToQTableWidget(QTableWidget *tableWidget)
{
    for(int i = 0; i < size; i++)
    {
        // Строки элементов таблицы
        QString indexString = QString::number(i);
        QString hash1String = ((table[i].hash1 != -1) ? QString::number(table[i].hash1) : "");
        QString hash2String = ((table[i].hash2 != -1) ? QString::number(table[i].hash2) : "");
        QString keyString = ((table[i].key != nullptr) ? QString(*table[i].key) : "");
        QString valueString = ((table[i].value != -1) ? QString::number(table[i].value + 1) : "");
        QString statusString = QString::number(table[i].status);

        // Элементы строки
        QTableWidgetItem *indexItem = new QTableWidgetItem(indexString);
        QTableWidgetItem *hash1Item = new QTableWidgetItem(hash1String);
        QTableWidgetItem *hash2Item = new QTableWidgetItem(hash2String);
        QTableWidgetItem *keyItem = new QTableWidgetItem(keyString);
        QTableWidgetItem *valueItem = new QTableWidgetItem(valueString);
        QTableWidgetItem *statusItem = new QTableWidgetItem(statusString);

        // Устанавливаем флаг запрета редактирования для каждого элемента
        indexItem->setFlags(indexItem->flags() & ~Qt::ItemIsEditable);
        hash1Item->setFlags(hash1Item->flags() & ~Qt::ItemIsEditable);
        hash2Item->setFlags(hash2Item->flags() & ~Qt::ItemIsEditable);
        keyItem->setFlags(keyItem->flags() & ~Qt::ItemIsEditable);
        valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
        statusItem->setFlags(statusItem->flags() & ~Qt::ItemIsEditable);

        // Заносим строку в таблицу
        int rowIndex = tableWidget->rowCount();
        tableWidget->insertRow(rowIndex);
        tableWidget->setItem(rowIndex, 0, indexItem);
        tableWidget->setItem(rowIndex, 1, hash1Item);
        tableWidget->setItem(rowIndex, 2, hash2Item);
        tableWidget->setItem(rowIndex, 3, keyItem);
        tableWidget->setItem(rowIndex, 4, valueItem);
        tableWidget->setItem(rowIndex, 5, statusItem);
    }
}

template <typename Key>
bool table3::HashTable<Key>::remove(Key key)
{
    int hash = hash1(key);
    int index = hash;

    int j = 1;
    while (table[index].key && *table[index].key != key)
    {
        if (j >= size)
            return false;
        index = hash2(hash, key, j);
        j++;
    }

    if (table[index].status == 1)
    {
        table[index].status = 0;

        count--;
        if (static_cast<double>(count) / size <= 0.3)
            resize(0.5);

        return true;
    }

    return false;
}

template <typename Key>
bool table3::HashTable<Key>::insert(Key key, int value)
{
    if (static_cast<double>(count) / size >= 0.7)
        resize(2);

    int hash = hash1(key);
    int index = hash;

    int j = 1;
    while (table[index].status == 1 && table[index].key && *table[index].key != key)
    {
        if (j >= size)
        {
            resize(2);
            j = 1;
            hash = hash1(key);
            index = hash;
            continue;
        }
        index = hash2(hash, key, j);
        j++;
    }

    if (table[index].status == 0)
    {
        if (table[index].key)
        {
            int searchIndex = index;
            while (table[searchIndex].key)
            {
                searchIndex = hash2(hash, key, j);
                j++;
                if (table[searchIndex].status == 1 && *table[searchIndex].key == key)
                    return false;
            }

            if(table[index].key)
                delete table[index].key;
        }

        Key* pkey = new Key(key);
        table[index].key = pkey;
        table[index].value = value;
        table[index].status = 1;
        table[index].hash1 = hash;  // Запоминаем hash1
        if(hash != index)
            table[index].hash2 = index; // Запоминаем hash2, если первычных хеш не равен вторичному
        count++;
        return true;
    }
}

template <typename Key>
typename table3::HashTable<Key>::Entry *table3::HashTable<Key>::find(Key key)
{
    int hash = hash1(key);
    int index = hash;

    int j = 1;
    if (table[index].status == 1 && *table[index].key == key)
        return &table[index];
    else
    {
        while (table[index].key && j < size)
        {
            index = hash2(hash, key, j);
            j++;
            if (table[index].status == 1 && *table[index].key == key)
                return &table[index];
        }
    }

    return nullptr; // Ключ не найден
}

template <typename Key>
bool table3::HashTable<Key>::isPrime(int number)
{
    if (number <= 1)
        return false;

    for (int i = 2; i * i <= number; ++i)
    {
        if (number % i == 0)
            return false;
    }

    return true;
}

template <typename Key>
void table3::HashTable<Key>::calculatePrimeSize()
{
    primeSize = size - 1;
    while (primeSize > 0 && !isPrime(primeSize))
        primeSize--;
}

template <typename Key>
int table3::HashTable<Key>::hash1(Key &key)
{
    int intKey = int(key);
    int sum = 0;
    while (intKey != 0)
    {
        sum += intKey % 10;
        intKey /= 10;
    }

    int sizeNumCount = 0;
    int sizeTmp = size;
    while (sizeTmp != 0)
    {
        sizeNumCount++;
        sizeTmp /= 10;
    }

    int hash = (sum * sum / 10) % ((int) pow(10, sizeNumCount));
    return hash % size;
}

template <typename Key>
int table3::HashTable<Key>::hash2(int hash1, int key, int j)
{
    return (hash1 + j * (1 + key % (primeSize))) % size;
}

template <typename Key>
void table3::HashTable<Key>::resize(double factor)
{
    int newSize = size * factor;
    if (newSize >= initialSize)
    {
        int oldSize = size;
        size = newSize;
        calculatePrimeSize();

        QVector<Entry> newTable(size);

        for (int i = 0; i < oldSize; ++i)
        {
            if (table[i].status == 1)
            {
                Key* key = table[i].key;
                int value = table[i].value;
                int hash = hash1(*key);
                int index = findEmptySlot(newTable, *key);
                newTable[index].key = key;
                newTable[index].value = value;
                newTable[index].hash1 = hash;  // Запоминаем hash1
                if(hash != index)
                    newTable[index].hash2 = index; // Запоминаем hash2, если первычных хеш не равен вторичному
                newTable[index].status = 1;
            }
            else if (table[i].status == 0 && table[i].key)
            {
                delete table[i].key;
                table[i].key = nullptr;
            }
        }

        table = std::move(newTable);
    }
}

template <typename Key>
int table3::HashTable<Key>::findEmptySlot(QVector<Entry> &table, Key key)
{
    int hash = hash1(key);
    int index = hash;

    int j = 1;
    while (table[index].status == 1 && table[index].key && *table[index].key != key)
    {
        if (j >= size)
            return -1;
        index = hash2(hash, key, j);
        j++;
    }

    if (table[index].status == 0)
    {
        if (table[index].key)
        {
            int searchIndex = index;
            while (table[searchIndex].key)
            {
                searchIndex = hash2(hash, key, j);
                j++;
                if (table[searchIndex].status == 1 && *table[searchIndex].key == key)
                    return -1;
            }
        }

        return index;
    }
}

template <typename Key>
void table3::RBTree<Key>::clear()
{
    this->clear(root);
    root = nullptr;
}

template <typename Key>
void table3::RBTree<Key>::clear(RBTreeNode<Key> *node)
{
    if(node == nullptr)
        return;

    this->clear(node->left);
    this->clear(node->right);

    delete node->valueList;
    delete node;
}

template <typename Key>
table3::RBTreeNode<Key> *table3::RBTree<Key>::findNode(Key key)
{
    table3::RBTreeNode<Key> *curr = root;
    table3::RBTreeNode<Key> *desired = nullptr;
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

template <typename Key>
void table3::RBTree<Key>::insertNode(Key key, int value)
{
    if (root == nullptr)
    {
        table3::DoublyLinkedRingList *valueList =
                new table3::DoublyLinkedRingList;
        valueList->insertNode(value);
        root = new RBTreeNode<Key>(key, valueList);
        insertNodeFixup(root);
    }
    else
    {
        RBTreeNode<Key> *current = root;
        RBTreeNode<Key> *parent = nullptr;
        bool appendList = false;
        while (current != nullptr && !appendList)
        {
            parent = current;
            if (key < current->key)
                current = current->left;
            else if(key > current->key)
                current = current->right;
            else
                appendList = true;
        }

        if(appendList)
            current->valueList->insertNode(value);
        else
        {
            table3::DoublyLinkedRingList *valueList =
                    new table3::DoublyLinkedRingList;
            valueList->insertNode(value);
            RBTreeNode<Key> *newNode = new RBTreeNode<Key>(key, valueList);
            newNode->parent = parent;
            if (key < parent->key)
                parent->left = newNode;
            else
                parent->right = newNode;
            insertNodeFixup(newNode);
        }
    }
}

template <typename Key>
void table3::RBTree<Key>::removeNode(Key key, int value)
{
    RBTreeNode<Key> *current = root;
    RBTreeNode<Key> *parent = nullptr;
    RBTreeNode<Key> *grandparent = nullptr;

    // Find the node to removeNode
    while (current != nullptr && current->key != key)
    {
        grandparent = parent;
        parent = current;

        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    if (current == nullptr)
        // Node with the given key doesn't exist
        return;

    // Удаление элемента из списка
    current->valueList->removeNode(value);
    if(current->valueList->isEmpty())
        delete current->valueList;
    else
        return;

    // Добавляю код от себя, в случае неправильного удаления - снестиt% 2#2 `B z2^`V `n t`×8j$r = w5$ ziD`V 5`C z[T7Y7zR1 2j$1F`B5^ ¬ 2t$j^c%
    if(current->parent == nullptr)
    {
        if(current->left == nullptr && current->right == nullptr)
        {
            delete current;
            root = nullptr;
            return;
        }
        else if(current->left == nullptr)
        {
            root = current->right;
            root->color = Color::BLACK;
            root->parent = nullptr;
            delete current;
            return;
        }
        else if(current->right == nullptr)
        {
            root = current->left;
            root->color = Color::BLACK;
            root->parent = nullptr;
            delete current;
            return;
        }
    }

    removeNode(current, parent, grandparent, *this);
}

template <typename Key>
QString table3::RBTree<Key>::getPrintableHtml(int l) const
{
    QString outputStr = getPrintableHtml(root, 0, l);
    return outputStr + "\n";
}

template <typename Key>
void table3::RBTree<Key>::insertNodeFixup(RBTreeNode<Key> *node)
{
    while (node->parent != nullptr && node->parent->color == Color::RED)
    {
        if (node->parent == node->parent->parent->left)
        {
            RBTreeNode<Key> *uncle = node->parent->parent->right;
            if (uncle != nullptr && uncle->color == Color::RED)
            {
                node->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateRight(node->parent->parent);
            }
        }
        else
        {
            RBTreeNode<Key> *uncle = node->parent->parent->left;
            if (uncle != nullptr && uncle->color == Color::RED)
            {
                node->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateLeft(node->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

template <typename Key>
void table3::RBTree<Key>::rotateLeft(RBTreeNode<Key> *node)
{
    RBTreeNode<Key> *rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nullptr)
        rightChild->left->parent = node;
    rightChild->parent = node->parent;
    if (node->parent == nullptr)
        root = rightChild;
    else if (node == node->parent->left)
        node->parent->left = rightChild;
    else
        node->parent->right = rightChild;
    rightChild->left = node;
    node->parent = rightChild;
}

template <typename Key>
void table3::RBTree<Key>::rotateRight(RBTreeNode<Key> *node)
{
    RBTreeNode<Key> *leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != nullptr)
        leftChild->right->parent = node;
    leftChild->parent = node->parent;
    if (node->parent == nullptr)
        root = leftChild;
    else if (node == node->parent->right)
        node->parent->right = leftChild;
    else
        node->parent->left = leftChild;
    leftChild->right = node;
    node->parent = leftChild;
}

template <typename Key>
table3::RBTreeNode<Key> *table3::RBTree<Key>::minimumNode(
        RBTreeNode<Key> *node)
{
    while (node->left != nullptr)
        node = node->left;

    return node;
}

template <typename Key>
void table3::RBTree<Key>::removeNode(RBTreeNode<Key> *node,
                                            RBTreeNode<Key> *parent,
                                            RBTreeNode<Key> *grandparent,
                                            RBTree<Key> &tree)
{
    // Case 1: Node is a leaf or has only one child
    if (node->left == nullptr || node->right == nullptr)
    {
        RBTreeNode<Key> *child =
                (node->left != nullptr) ? node->left : node->right;

        // Replace the node with its child
        if (node == parent->left)
            parent->left = child;
        else
            parent->right = child;

        if (child != nullptr)
            child->parent = parent;

        if (node->color == Color::BLACK)
        {
            // Case 1.1: Node is black and its child is red
            if (child != nullptr && child->color == Color::RED)
                child->color = Color::BLACK;
            else
                // Case 1.2: Node is black and its child is black
                deleteNodeFixup(child, parent, grandparent, tree);
        }

        delete node;
        return;
    }

    // Case 2: Node has two children
    // Find the successor (minimum value in the right subtree)
    RBTreeNode<Key> *successor = minimumNode(node->right);
//            Node<Key, Value> *successor = minimumNode(node->left);

    // Copy the successor's key and value to the node
    node->key = successor->key;
    node->valueList = successor->valueList;

    // Recursively removeNode the successor node
    removeNode(successor, successor->parent, grandparent, tree);
}

template <typename Key>
void table3::RBTree<Key>::deleteNodeFixup(RBTreeNode<Key> *node,
                                                 RBTreeNode<Key> *parent,
                                                 RBTreeNode<Key> *grandparent,
                                                 RBTree<Key> &tree)
{
    while ((node == nullptr || node->color == Color::BLACK) && node != tree.root)
    {
        if (node == parent->left)
        {
            RBTreeNode<Key> *sibling = parent->right;

            // Case 1: Sibling is red
            if (sibling->color == Color::RED)
            {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                tree.rotateLeft(parent);
                sibling = parent->right;
            }

            // Case 2: Sibling is black and both of its children are black
            if ((sibling->left == nullptr || sibling->left->color == Color::BLACK) &&
                (sibling->right == nullptr || sibling->right->color == Color::BLACK))
            {
                sibling->color = Color::RED;
                node = parent;
                parent = node->parent;
                grandparent = (parent != nullptr) ? parent->parent : nullptr; // Update grandparent
            }
            else
            {
                // Case 3: Sibling is black and its right child is black
                if (sibling->right == nullptr || sibling->right->color == Color::BLACK)
                {
                    sibling->left->color = Color::BLACK;
                    sibling->color = Color::RED;
                    tree.rotateRight(sibling);
                    sibling = parent->right;
                }

                // Case 4: Sibling is black and its right child is red
                sibling->color = parent->color;
                parent->color = Color::BLACK;
                sibling->right->color = Color::BLACK;
                tree.rotateLeft(parent);
                node = tree.root;
                break;
            }
        }
        else
        {
            RBTreeNode<Key> *sibling = parent->left;

            // Case 1: Sibling is red
            if (sibling->color == Color::RED)
            {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                tree.rotateRight(parent);
                sibling = parent->left;
            }

            // Case 2: Sibling is black and both of its children are black
            if ((sibling->left == nullptr || sibling->left->color == Color::BLACK) &&
                (sibling->right == nullptr || sibling->right->color == Color::BLACK))
            {
                sibling->color = Color::RED;
                node = parent;
                parent = node->parent;
                grandparent = (parent != nullptr) ? parent->parent : nullptr; // Update grandparent
            }
            else
            {
                // Case 3: Sibling is black and its left child is black
                if (sibling->left == nullptr || sibling->left->color == Color::BLACK)
                {
                    sibling->right->color = Color::BLACK;
                    sibling->color = Color::RED;
                    tree.rotateLeft(sibling);
                    sibling = parent->left;
                }

                // Case 4: Sibling is black and its left child is red
                sibling->color = parent->color;
                parent->color = Color::BLACK;
                sibling->left->color = Color::BLACK;
                tree.rotateRight(parent);
                node = tree.root;
                break;
            }
        }
    }

    if (node != nullptr)
        node->color = Color::BLACK;
}

template <typename Key>
QString table3::RBTree<Key>::getPrintableHtml(RBTreeNode<Key> *node,
                                                     int h, int l) const
{
    if (node != nullptr)
    {
        QString outputStr = getPrintableHtml(node->right, h + l, l);
        QString spaceSymbol(" ");
        QString listPrintableString = node->valueList->getPrintableString();
        if(node->color == Color::RED)
            outputStr+= "<p style=\"color:red; white-space:pre\">" +
                    spaceSymbol.repeated(h) + QVariant(node->key).toString() +
                    " [" + listPrintableString + "]</p>";
        else
            outputStr+= "<p style=\"color:black; white-space:pre\">" +
                    spaceSymbol.repeated(h) + QVariant(node->key).toString() +
                    " [" + listPrintableString + "]</p>";
        outputStr += getPrintableHtml(node->left, h + l, l);
        return outputStr;
    }
    return "";
}

#endif // TABLE3_H
