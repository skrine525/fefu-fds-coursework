#ifndef TABLE3_H
#define TABLE3_H

#include <QString>
#include <QVector>

namespace table3
{
    struct Fullname
    {
        QString firstName;
        QString lastName;
        QString middleName;
        int phoneNumber;

        Fullname() {}
        Fullname(QString firstName, QString lastName,
                 QString middleName, int phoneNumber);

        operator QString() const;
    };
    bool operator<(const Fullname& a, const Fullname& b);
    bool operator>(const Fullname& a, const Fullname& b);
    bool operator==(const Fullname& a, const Fullname& b);
    bool operator>=(const Fullname& a, const Fullname& b);
    bool operator<=(const Fullname& a, const Fullname& b);
    bool operator!=(const Fullname& a, const Fullname& b);

    struct Datetime
    {
        unsigned year;
        unsigned month;
        unsigned day;
        unsigned hour;
        unsigned minute;

        operator QString() const;
    };

    struct Record
    {
        long long doctorPhoneNumber;
        long long patientPhoneNumber;
        Datetime appointmentDatetime;
        unsigned appointmentCost;
    };

    enum class Color { RED, BLACK };

    template <typename Key, typename Value>
    struct RBTreeNode
    {
        Key key;
        Value value;
        Color color;
        RBTreeNode* left;
        RBTreeNode* right;
        RBTreeNode* parent;

        explicit RBTreeNode(Key k, Value v, Color c = Color::RED)
            : key(k), value(v), color(c), left(nullptr),
              right(nullptr), parent(nullptr) {}
    };

    template <typename Key, typename Value>
    class RBTree
    {
    public:
        RBTree() : root(nullptr) {}

        void insertNode(Key key, Value value);
        void deleteNode(Key key);
        QString getPrintableHtml(int l) const;

    private:
        RBTreeNode<Key, Value>* root;

        void insertNodeFixup(RBTreeNode<Key, Value>* node);
        void rotateLeft(RBTreeNode<Key, Value>* node);
        void rotateRight(RBTreeNode<Key, Value>* node);
        RBTreeNode<Key, Value>* minimumNode(RBTreeNode<Key, Value>* node);
        void deleteNode(RBTreeNode<Key, Value>* node,
                        RBTreeNode<Key, Value>* parent,
                        RBTreeNode<Key, Value>* grandparent, RBTree<Key, Value>& tree);
        void deleteNodeFixup(RBTreeNode<Key, Value>* node,
                             RBTreeNode<Key, Value>* parent,
                             RBTreeNode<Key, Value>* grandparent, RBTree<Key,
                             Value>& tree);
        QString getPrintableHtml(RBTreeNode<Key, Value>* node, int h, int l) const;
    };

    struct Appointments
    {
        QVector<Record> records;
    };
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::insertNode(Key key, Value value)
{
    RBTreeNode<Key, Value>* newNode = new RBTreeNode<Key, Value>(key, value);
    if (root == nullptr)
        root = newNode;
    else
    {
        RBTreeNode<Key, Value>* current = root;
        RBTreeNode<Key, Value>* parent = nullptr;
        while (current != nullptr)
        {
            parent = current;
            if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        newNode->parent = parent;
        if (key < parent->key)
            parent->left = newNode;
        else
            parent->right = newNode;
    }
    insertNodeFixup(newNode);
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::deleteNode(Key key)
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

    deleteNode(current, parent, grandparent, *this);
}

template <typename Key, typename Value>
QString table3::RBTree<Key, Value>::getPrintableHtml(int l) const
{
    QString outputStr = getPrintableHtml(root, 0, l);
    return outputStr + "\n";
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::insertNodeFixup(RBTreeNode<Key, Value>* node)
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
                    rotateLeft(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateRight(node->parent->parent);
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

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::rotateLeft(RBTreeNode<Key, Value>* node)
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
void table3::RBTree<Key, Value>::rotateRight(RBTreeNode<Key, Value>* node)
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
void table3::RBTree<Key, Value>::deleteNode(RBTreeNode<Key, Value>* node,
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
                deleteNodeFixup(child, parent, grandparent, tree);
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
    node->value = successor->value;

    // Recursively delete the successor node
    deleteNode(successor, successor->parent, grandparent, tree);
}

template <typename Key, typename Value>
void table3::RBTree<Key, Value>::deleteNodeFixup(RBTreeNode<Key, Value>* node,
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
            RBTreeNode<Key, Value>* sibling = parent->left;

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

template <typename Key, typename Value>
QString table3::RBTree<Key, Value>::getPrintableHtml(RBTreeNode<Key, Value>* node,
                                                     int h, int l) const
{
    if (node != nullptr) {
        QString outputStr = getPrintableHtml(node->right, h + l, l);
        QString spaceSymbol(" ");
        if(node->color == Color::RED)
            outputStr+= "<p style=\"color:red; white-space:pre\">" +
                    spaceSymbol.repeated(h) + node->key + "</p>";
        else
            outputStr+= "<p style=\"color:black; white-space:pre\">" +
                    spaceSymbol.repeated(h) + node->key + "</p>";
        outputStr += getPrintableHtml(node->left, h + l, l);
        return outputStr;
    }
    return "";
}


#endif // TABLE3_H
