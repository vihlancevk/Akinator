#include "Tree.h"
#include "Stack.h"
#include "UnitTests.h"

#define DEBUG
//#undef DEBUG

int main()
{
    #ifdef DEBUG
        ClearLogFile();
    #endif // DEBUG

    TreeErrorCode treeError = TREE_NO_ERROR;

    //treeError = UnitTestForTree();

    //UnitTestForGuessObject();

    //UnitTestForDefineObjectMode();

    //UnitTestForCompairObjectsMode();

    //UnitTestForShowTree();

    return treeError;
}
