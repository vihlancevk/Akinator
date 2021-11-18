#include "UnitTests.h"
#include "AkinatorMode.h"

#define DEBUG

const int STACK_SIZE = 10;

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

void UnitTestForGuessObject()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    GuessObject(&tree);

    TreeDtor(&tree);
}

void UnitTestForDefineObjectMode()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    stack_t stack = {};
    STACKCTOR_(&stack, STACK_SIZE);
    stack = *DefineObjectMode(&tree, "Лев", &stack);
    StackDtor(&stack);

    TreeDtor(&tree);
}

void UnitTestForCompairObjectsMode()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    CompairObjectsMode(&tree, "Андрей", "Ясин");

    TreeDtor(&tree);
}

void UnitTestForShowTree()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    ShowTree(&tree);

    TreeDtor(&tree);
}
