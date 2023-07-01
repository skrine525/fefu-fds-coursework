#ifndef TABLE3_H
#define TABLE3_H

#include <QString>
#include <QVector>
#include <QVariant>

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
        void deleteNode(int value);
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
        void deleteNode(Key key, int value);
        RBTreeNode<Key> *findNode(Key key);
        QString getPrintableHtml(int l) const;
        void clear();

    private:
        RBTreeNode<Key> *root;

        void insertNodeFixup(RBTreeNode<Key> *node);
        void rotateLeft(RBTreeNode<Key> *node);
        void rotateRight(RBTreeNode<Key> *node);
        RBTreeNode<Key> *minimumNode(RBTreeNode<Key> *node);
        void deleteNode(RBTreeNode<Key> *node,
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
    };
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
void table3::RBTree<Key>::deleteNode(Key key, int value)
{
    RBTreeNode<Key> *current = root;
    RBTreeNode<Key> *parent = nullptr;
    RBTreeNode<Key> *grandparent = nullptr;

    // Find the node to deleteNode
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
    current->valueList->deleteNode(value);
    if(current->valueList->isEmpty())
        delete current->valueList;
    else
        return;

    // Добавляю код от себя, в случае неправильного удаления - снестиt% 2#2 `B z2^`V `N t`×8j$r = w5$ ziD`V 5`C z[T7Y7zR1 2j$1F`B5^ ¬ 2t$j^c%
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

    deleteNode(current, parent, grandparent, *this);
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
void table3::RBTree<Key>::deleteNode(RBTreeNode<Key> *node,
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

    // Recursively deleteNode the successor node
    deleteNode(successor, successor->parent, grandparent, tree);
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
