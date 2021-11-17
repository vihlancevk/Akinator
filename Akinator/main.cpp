#include "Tree.h"
#include "Stack.h"
#include "UnitTests.h"

int main()
{
    ClearLogFile();

    TreeErrorCode treeError = TREE_NO_ERROR;

    //treeError = UnitTestForTree();

    //UnitTestForQuessingMode();

    UnitTestForObjectDefinitionMode();

    //UnitTestForObjectComparisonMode();

    //UnitTestForTreeShow();

    return treeError;
}
