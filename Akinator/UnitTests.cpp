#include "UnitTests.h"

TreeErrorCode UnitTestForTree()
{
    Tree_t tree = {};

    TreeErrorCode treeError = TreeCtor(&tree);

    Node_t *newNode = TreeInsert(&tree, tree.root, "Is it an animal?", LEFT_CHILD, &treeError);

    Node_t *newNode1 = TreeInsert(&tree, newNode, "Does it live in your room?", LEFT_CHILD, &treeError);
    TreeInsert(&tree, newNode1, "This is Yura!", LEFT_CHILD, &treeError);
    TreeInsert(&tree, newNode1, "This is Vanya!", RIGHT_CHILD, &treeError);

    TreeInsert(&tree, newNode, "Poltarashka!", RIGHT_CHILD, &treeError);

    TreeDtor(&tree);

    return treeError;
}
