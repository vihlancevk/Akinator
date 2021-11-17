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

void UnitTestForQuessingMode()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    QuessingMode(&tree);

    TreeDtor(&tree);
}

void UnitTestForObjectDefinitionMode()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    stack_t stack = {};
    STACKCTOR_(&stack, STACK_SIZE);
    stack = *ObjectDefinitionMode(&tree, "Лев", &stack);
    StackDtor(&stack);

    TreeDtor(&tree);
}

void UnitTestForObjectComparisonMode()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    ObjectComparisonMode(&tree, "Андрей", "Ясин");

    TreeDtor(&tree);
}

void UnitTestForTreeShow()
{
    Tree_t tree = {};

    TreeCtor(&tree);

    TreeBuild(&tree);

    TreeShow(&tree);

    TreeDtor(&tree);
}
