#include "UnitTests.h"

TreeErrorCode UnitTestForTree()
{
    Tree_t tree = {};

    TreeErrorCode treeError = TreeCtor(&tree);

    Node_t *newNode = TreeInsert(&tree, tree.beginTree, "Is it an animal?", 1, &treeError);

    Node_t *newNode1 = TreeInsert(&tree, newNode, "Does it live in your room?", 1, &treeError);
    TreeInsert(&tree, newNode1, "This is Yura!", 1, &treeError);
    TreeInsert(&tree, newNode1, "This is Vanya!", 0, &treeError);

    TreeInsert(&tree, newNode, "Poltarashka!", 0, &treeError);

    TreeDtor(&tree);

    return treeError;
}
