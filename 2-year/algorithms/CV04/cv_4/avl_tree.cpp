#include "avl_tree.h"
#include <algorithm> 

// Constructor for Node
Node::Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}

// Constructor for AVLTree
AVLTree::AVLTree() : root(nullptr) {}

// Public method to insert a key into the AVL tree
void AVLTree::insert(int key) {
    root = insert(root, key);
}

// Public method to delete a key from the AVL tree
void AVLTree::deleteKey(int key) {
    root = deleteNode(root, key);
}

// Function to get the height of the tree
int AVLTree::height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Right rotate subtree rooted with y
Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));

    // Return new root
    return x;
}

// Left rotate subtree rooted with x
Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));

    // Return new root
    return y;
}

// Get balance factor of node
int AVLTree::getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

// Recursive function to insert a key in the subtree rooted with node
Node* AVLTree::insert(Node* node, int key) {
    // Perform the normal BST insertion
    if (node == nullptr)
        return new Node(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;  // Equal keys are not allowed in the AVL tree

    // Update height of this ancestor node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // Get the balance factor of this node
    int balance = getBalance(node);

    // Balance the tree
    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Utility function to find the node with the minimum key value
Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Recursive function to delete a node with the given key
Node* AVLTree::deleteNode(Node* root, int key) {
    // Perform the standard BST delete
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Node with only one child or no child
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp; // One child case
            }
            delete temp;
        } else {
            // Node with two children: Get the inorder successor (smallest in right subtree)
            Node* temp = minValueNode(root->right);

            // Copy the inorder successor's key
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node, return
    if (root == nullptr)
        return root;

    // Update height of the current node
    root->height = 1 + std::max(height(root->left), height(root->right));

    // Get the balance factor to check if it became unbalanced
    int balance = getBalance(root);

    // Balance the tree
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

bool AVLTree::find_element(int key) {

   return find_element(this->root, key);

}

bool AVLTree::find_element(Node* root, int key) {


    if (root == nullptr) {
        return false;
    }

    if (root->key == key) {

        return true;
    }

    if (root->key > key) {

        return find_element(root->left, key);

    }
    else {
    
        return find_element(root->right, key);
    }

}

void AVLTree::print_all_elementes() {

    print_all_elementes(this->root);

}

void AVLTree::print_all_elementes(Node* root) {

    if (root != nullptr) {
        print_all_elementes(root->left);
        std::cout << root->key << "  ";
        print_all_elementes(root->right);
    }

}


void AVLTree::print_tree_height() {

     print_tree_height(this->root);
}


void AVLTree::print_tree_height(Node* root) {

    std::cout << root->height << std::endl;

}

void AVLTree::get_all_nodes_info(){

    get_all_nodes_info(this->root);
}

void AVLTree::get_all_nodes_info(Node* root) {


    if (root == nullptr) {

        return;
    }

    get_all_nodes_info(root->left);

    std::cout << root->key << " " << root->height << " " << getBalance(root) << std::endl;

    get_all_nodes_info(root->right);
}



