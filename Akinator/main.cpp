#include <stdio.h>
#include "Tree.h"
#include "UnitTests.h"

int main()
{
    TreeErrorCode treeError = TREE_NO_ERROR;

    //treeError = UnitTestForTree();

    Tree_t tree = {};
    treeError = TreeCtor(&tree);
    treeError = TreeFill(&tree);

    return treeError;
}
