#include "Tree.h"
#include "Stack.h"
#include "UnitTests.h"

int main()
{
    ClearLogFile();

    TreeErrorCode treeError = TREE_NO_ERROR;

    //treeError = UnitTestForTree();

    Tree_t tree = {};
    treeError = TreeCtor(&tree);

    treeError = TreeBuild(&tree);

    //UnitTestForQuessingMode();

    //UnitTestForObjectDefinitionMode();

    treeError = TreeDtor(&tree);

    return treeError;
}
