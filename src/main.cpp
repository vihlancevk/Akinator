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

    /*while (1)
    {
        system("espeak-ng -s 150 -v tt \"Ты запостил кринж!\"");
    }*/

    //treeError = UnitTestForTree();

    //UnitTestForGuessObject();

    //UnitTestForDefineObjectMode();

    //UnitTestForCompairObjectsMode();

    //UnitTestForShowTree();

    return treeError;
}
