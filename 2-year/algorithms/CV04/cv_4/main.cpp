#include "avl_tree.h"

int main()
{
    AVLTree tree;

    tree.insert(12);
    tree.insert(11);
    tree.insert(9);
    tree.insert(1);
    tree.insert(16);
    tree.insert(6);
    tree.insert(8);
    tree.insert(26);

    std::cout << tree.find_element(900) << std::endl;
   
    tree.print_all_elementes();
    std::cout << std::endl;
    tree.print_tree_height();
    std::cout << std::endl;

    tree.get_all_nodes_info();

    return 0;
}
