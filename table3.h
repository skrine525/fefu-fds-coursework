#ifndef TABLE3_H
#define TABLE3_H

#include <QString>
#include <QVector>
#include <QDebug>

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
    bool operator<(const Datetime& a, const Datetime& b);
    bool operator>(const Datetime& a, const Datetime& b);
    bool operator==(const Datetime& a, const Datetime& b);

    struct Record
    {
        long long doctorPhoneNumber;
        long long patientPhoneNumber;
        Datetime appointmentDatetime;
        unsigned appointmentCost;
    };

    template <typename Value>
    struct DoublyLinkedRingListNode
    {
        DoublyLinkedRingListNode* next;
        DoublyLinkedRingListNode* prev;
        Value value;
    };

    template <typename Value>
    class DoublyLinkedRingList
    {
    public:
        DoublyLinkedRingList() : head(nullptr) {}
        ~DoublyLinkedRingList();
        void Insert(Value value);
        void Delete(Value value);
        QString getPrintableString();
        bool IsEmpty();
        void Clear();

    private:
        DoublyLinkedRingListNode<Value> *head;
    };

    enum class Color { RED, BLACK };

    template <typename Key, typename Value>
    struct RBTreeNode
    {
        Key key;
        DoublyLinkedRingList<Value> *valueList;
        Color color;
        RBTreeNode* left;
        RBTreeNode* right;
        RBTreeNode* parent;

        explicit RBTreeNode(Key k, DoublyLinkedRingList<Value> *v, Color c = Color::RED)
            : key(k), valueList(v), color(c), left(nullptr),
              right(nullptr), parent(nullptr) {}
    };

    template <typename Key, typename Value>
    class RBTree
    {
    public:
        RBTree() : root(nullptr) {}

        void InsertNode(Key key, Value value);
        void DeleteNode(Key key, Value value);
        QString GetPrintableHtml(int l) const;

    private:
        RBTreeNode<Key, Value> *root;

        void InsertNodeFixup(RBTreeNode<Key, Value>* node);
        void RotateLeft(RBTreeNode<Key, Value>* node);
        void RotateRight(RBTreeNode<Key, Value>* node);
        RBTreeNode<Key, Value>* minimumNode(RBTreeNode<Key, Value>* node);
        void DeleteNode(RBTreeNode<Key, Value>* node,
                        RBTreeNode<Key, Value>* parent,
                        RBTreeNode<Key, Value>* grandparent, RBTree<Key, Value>& tree);
        void DeleteNodeFixup(RBTreeNode<Key, Value>* node,
                             RBTreeNode<Key, Value>* parent,
                             RBTreeNode<Key, Value>* grandparent, RBTree<Key,
                             Value>& tree);
        QString GetPrintableHtml(RBTreeNode<Key, Value>* node, int h, int l) const;
    };

    struct Appointments
    {
        QVector<Record> records;
        RBTree<long long, int> doctorPhoneNumberTree;
        RBTree<long long, int> patientPhoneNumberTree;
        RBTree<Datetime, int> appointmentDatetimeTree;
        RBTree<unsigned, int> appointmentCostTree;
    };
}

template <typename Value>
table3::DoublyLinkedRingList<Value>::~DoublyLinkedRingList()
{
    qDebug() << "List Destroy";
    this->Clear();
}

template <typename Value>
void table3::DoublyLinkedRingList<Value>::Clear()
{
    table3::DoublyLinkedRingListNode<Value> *curr = head;
    if (curr != nullptr)
    {
        do
        {
            table3::DoublyLinkedRingListNode<Value> *d = curr;
            curr = curr->next;
            delete d;
        } while (curr != head);
    }
    head = nullptr;
}

template <typename Value>
bool table3::DoublyLinkedRingList<Value>::IsEmpty()
{
    return (head == nullptr);
}

template <typename Value>
QString table3::DoublyLinkedRingList<Value>::getPrintableString()
{
    QString str;
    table3::DoublyLinkedRingListNode<Value> *curr = head;

    if (curr != nullptr)
    {
        do
        {
            str += QVariant(curr->value).toString();
            curr = curr->next;
            if (curr != head)
                str += " ";
        } while (curr != head);
    }
    return str;
}

template <typename Value>
void table3::DoublyLinkedRingList<Value>::Delete(Value value)
{
    table3::DoublyLinkedRingListNode<Value> *curr = head;
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
                    table3::DoublyLinkedRingListNode<Value> *prev = curr->prev;
                    table3::DoublyLinkedRingListNode<Value> *next = curr->next;
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

template <typename Value>
void table3::DoublyLinkedRingList<Value>::Insert(Value value)
{
    if (head == nullptr)
    {
        head = new table3::DoublyLinkedRingListNode<Value>;
        head->next = head;
        head->prev = head;
        head->value = value;
    }
    else
    {
        table3::DoublyLinkedRingListNode<Value> *curr = head;
        do
        {
            if (curr->value != value && value < curr->value)
            {
                table3::DoublyLinkedRingListNode<Value> *ncurr
                        = new table3::DoublyLinkedRingListNode<Value>;
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
                table3::DoublyLinkedRingListNode<Value> *ncurr
                        = new table3::DoublyLinkedRingListNode<Value>;
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

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::InsertNode(Key key, Value value)
{
    if (root == nullptr)
    {
        table3::DoublyLinkedRingList<Value> *valueList =
                new table3::DoublyLinkedRingList<Value>;
        valueList->Insert(value);
        root = new RBTreeNode<Key, Value>(key, valueList);
        InsertNodeFixup(root);
    }
    else
    {
        RBTreeNode<Key, Value>* current = root;
        RBTreeNode<Key, Value>* parent = nullptr;
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
            current->valueList->Insert(value);
        else
        {
            table3::DoublyLinkedRingList<Value> *valueList =
                    new table3::DoublyLinkedRingList<Value>;
            valueList->Insert(value);
            RBTreeNode<Key, Value> *newNode = new RBTreeNode<Key, Value>(key, valueList);
            newNode->parent = parent;
            if (key < parent->key)
                parent->left = newNode;
            else
                parent->right = newNode;
            InsertNodeFixup(newNode);
        }
    }
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::DeleteNode(Key key, Value value)
{
    RBTreeNode<Key, Value>* current = root;
    RBTreeNode<Key, Value>* parent = nullptr;
    RBTreeNode<Key, Value>* grandparent = nullptr;

    // Find the node to delete
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
    current->valueList->Delete(value);
    if(current->valueList->IsEmpty())
        delete current->valueList;
    else
        return;

    // Добавляю код от себя, в случае неправильного удаления - снести
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

    DeleteNode(current, parent, grandparent, *this);
}

template <typename Key, typename Value>
QString table3::RBTree<Key, Value>::GetPrintableHtml(int l) const
{
    QString outputStr = GetPrintableHtml(root, 0, l);
    return outputStr + "\n";
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::InsertNodeFixup(RBTreeNode<Key, Value>* node)
{
    while (node->parent != nullptr && node->parent->color == Color::RED)
    {
        if (node->parent == node->parent->parent->left)
        {
            RBTreeNode<Key, Value>* uncle = node->parent->parent->right;
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
                    RotateLeft(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                RotateRight(node->parent->parent);
            }
        }
        else
        {
            RBTreeNode<Key, Value>* uncle = node->parent->parent->left;
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
                    RotateRight(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                RotateLeft(node->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::RotateLeft(RBTreeNode<Key, Value>* node)
{
    RBTreeNode<Key, Value>* rightChild = node->right;
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

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::RotateRight(RBTreeNode<Key, Value>* node)
{
    RBTreeNode<Key, Value>* leftChild = node->left;
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

template <typename Key, typename Value>
table3::RBTreeNode<Key, Value>* table3::RBTree<Key, Value>::minimumNode(
        RBTreeNode<Key, Value>* node)
{
    while (node->left != nullptr)
        node = node->left;

    return node;
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::DeleteNode(RBTreeNode<Key, Value>* node,
                                            RBTreeNode<Key, Value>* parent,
                                            RBTreeNode<Key, Value>* grandparent,
                                            RBTree<Key, Value>& tree)
{
    // Case 1: Node is a leaf or has only one child
    if (node->left == nullptr || node->right == nullptr)
    {
        RBTreeNode<Key, Value>* child =
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
                DeleteNodeFixup(child, parent, grandparent, tree);
        }

        delete node;
        return;
    }

    // Case 2: Node has two children
    // Find the successor (minimum value in the right subtree)
    RBTreeNode<Key, Value>* successor = minimumNode(node->right);
//            Node<Key, Value>* successor = minimumNode(node->left);

    // Copy the successor's key and value to the node
    node->key = successor->key;
    node->valueList = successor->valueList;

    // Recursively delete the successor node
    DeleteNode(successor, successor->parent, grandparent, tree);
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::DeleteNodeFixup(RBTreeNode<Key, Value>* node,
                                                 RBTreeNode<Key, Value>* parent,
                                                 RBTreeNode<Key, Value>* grandparent,
                                                 RBTree<Key, Value>& tree)
{
    while ((node == nullptr || node->color == Color::BLACK) && node != tree.root)
    {
        if (node == parent->left)
        {
            RBTreeNode<Key, Value>* sibling = parent->right;

            // Case 1: Sibling is red
            if (sibling->color == Color::RED)
            {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                tree.RotateLeft(parent);
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
                    tree.RotateRight(sibling);
                    sibling = parent->right;
                }

                // Case 4: Sibling is black and its right child is red
                sibling->color = parent->color;
                parent->color = Color::BLACK;
                sibling->right->color = Color::BLACK;
                tree.RotateLeft(parent);
                node = tree.root;
                break;
            }
        }
        else
        {
            RBTreeNode<Key, Value>* sibling = parent->left;

            // Case 1: Sibling is red
            if (sibling->color == Color::RED)
            {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                tree.RotateRight(parent);
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
                    tree.RotateLeft(sibling);
                    sibling = parent->left;
                }

                // Case 4: Sibling is black and its left child is red
                sibling->color = parent->color;
                parent->color = Color::BLACK;
                sibling->left->color = Color::BLACK;
                tree.RotateRight(parent);
                node = tree.root;
                break;
            }
        }
    }

    if (node != nullptr)
        node->color = Color::BLACK;
}

template <typename Key, typename Value>
QString table3::RBTree<Key, Value>::GetPrintableHtml(RBTreeNode<Key, Value>* node,
                                                     int h, int l) const
{
    if (node != nullptr) {
        QString outputStr = GetPrintableHtml(node->right, h + l, l);
        QString spaceSymbol(" ");
        QString listPrintableString = node->valueList->getPrintableString();
        if(node->color == Color::RED)
            outputStr+= "<p style=\"color:red; white-space:pre\">" +
                    spaceSymbol.repeated(h) + QVariant(node->key).toString() +
                    " [" + listPrintableString + "]" "</p>";
        else
            outputStr+= "<p style=\"color:black; white-space:pre\">" +
                    spaceSymbol.repeated(h) + QVariant(node->key).toString() +
                    " [" + listPrintableString + "]" "</p>";
        outputStr += GetPrintableHtml(node->left, h + l, l);
        return outputStr;
    }
    return "";
}


#endif // TABLE3_H
