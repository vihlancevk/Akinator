#ifndef TREE_H_
#define TREE_H_

typedef char* elemT;

enum TreeStatus
{
    TREE_NOT_CONSTRUCTED,
    TREE_CONSTRUCTED,
    TREE_DESTRUCTED,
};

enum TreeErrorCode
{
    TREE_NO_ERROR,
    TREE_CONSTRUCTED_ERROR,
    TREE_INSERT_ERROR,
    TREE_DESTRUCTED_ERROR,
};

struct Node_t
{
    elemT elem;
    Node_t *left;
    Node_t *right;
    size_t num;
};

struct Tree_t
{
    Node_t *beginTree;
    size_t size;
    TreeStatus status;
};

void TreeDump(Tree_t *tree);

TreeErrorCode TreeCtor(Tree_t *tree);

Node_t* TreeInsert(Tree_t *tree, Node_t *node, char *str, const int isLeft, TreeErrorCode *treeError);

TreeErrorCode TreeDtor(Tree_t *tree);

#endif // TREE_H_
