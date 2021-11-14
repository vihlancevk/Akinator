#include <stdio.h>
#include "Tree.h"

int main()
{
    Tree_t tree = {};

    TreeErrorCode treeError = TREE_NO_ERROR;

    TreeCtor(&tree);

    Node_t *newNode = TreeInsert(&tree, tree.beginTree, "Is it an animal?", 1, &treeError);

    TreeInsert(&tree, newNode, "No Poltarashka!", 1, &treeError);

    TreeInsert(&tree, newNode, "Poltarashka!", 0, &treeError);

    TreeDtor(&tree);

    return treeError;
}
