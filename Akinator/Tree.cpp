#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Tree.h"

#define DEBUG

const size_t STR_MAX_SIZE = 100;
const char *TREE_GRAPH_VIZ = "graphviz.gv";

static void TreeVisitFprintfNode(const Node_t *node, FILE *foutput)
{
    if (node == nullptr) return;
    assert(foutput != nullptr);

    fprintf(foutput, "\t%lu[shape=record, style=\"filled\", fillcolor=\"yellow\", label=\"<%lu> %s\"];\n", node->num, node->num, node->elem);

    if (node->left != nullptr) TreeVisitFprintfNode(node->left, foutput);
    if (node->right != nullptr) TreeVisitFprintfNode(node->right, foutput);
}

static void TreeVisitFprintfArrow(const Node_t *node, FILE *foutput)
{
    if (node == nullptr) return;
    assert(foutput != nullptr);

    if (node->left != nullptr && node->right != nullptr)
    {
        fprintf(foutput, "\t%lu -> %lu[color=\"green\", label=\"NO\", fontsize=12]\n", node->num, node->left->num);
        fprintf(foutput, "\t%lu -> %lu[color=\"blue\", label=\"YES\", fontsize=12]\n", node->num, node->right->num);
    }

    if (node->left != nullptr) TreeVisitFprintfArrow(node->left, foutput);
    if (node->right != nullptr) TreeVisitFprintfArrow(node->right, foutput);
}

void TreeDump(Tree_t *tree)
{
    FILE *graphViz = fopen(TREE_GRAPH_VIZ, "w");

    fprintf(graphViz, "digraph Tree{\n\n");
    fprintf(graphViz,"\trankdir=UD;\n\n");
    fprintf(graphViz, "\tnode[color=\"red\",fontsize=14];\n\n");

    TreeVisitFprintfNode(tree->beginTree, graphViz);

    fprintf(graphViz, "\n");

    TreeVisitFprintfArrow(tree->beginTree, graphViz);

    fprintf(graphViz, "\n");

    fprintf(graphViz, "}");

    fclose(graphViz);

    system("dot -Tpng /home/kostya/Akinator/graphviz.gv -o /home/kostya/Akinator/graphviz.png");
}

TreeErrorCode TreeCtor(Tree_t *tree)
{
    assert(tree != nullptr);

    TreeErrorCode treeError = TREE_NO_ERROR;

    if (tree->status != TREE_NOT_CONSTRUCTED)
    {
        treeError = TREE_CONSTRUCTED_ERROR;
    }

    tree->beginTree = (Node_t*)calloc(1, sizeof(Node_t));
    if (tree->beginTree == nullptr)
    {
        treeError = TREE_CONSTRUCTED_ERROR;
    }
    tree->beginTree->elem = (char*)calloc(STR_MAX_SIZE, sizeof(char));
    if (tree->beginTree->elem == nullptr)
    {
        treeError = TREE_CONSTRUCTED_ERROR;
    }
    strcpy(tree->beginTree->elem, "Do we know who?");
    tree->beginTree->num = 1;

    tree->size = 0;
    tree->status = TREE_CONSTRUCTED;

    #ifdef DEBUG
        TreeDump(tree);
    #endif // DEBUG

    return treeError;
}

static void NodeDtor(Node_t *node)
{
    assert(node != nullptr);

    Node_t *left = node->left;
    Node_t *right = node->right;
    free(node->elem);
    free(node);

    if (left != nullptr) NodeDtor(left);
    if (right != nullptr) NodeDtor(right);
}

TreeErrorCode TreeDtor(Tree_t *tree)
{
    assert(tree != nullptr);

    if (tree->status == TREE_DESTRUCTED)
    {
        return TREE_DESTRUCTED_ERROR;
    }

    NodeDtor(tree->beginTree);
    tree->size = 0;
    tree->size = TREE_DESTRUCTED;

    return TREE_NO_ERROR;
}

Node_t* TreeInsert(Tree_t *tree, Node_t *node, char *str, const int isLeft, TreeErrorCode *treeError)
{
    assert(tree != nullptr);
    assert(node != nullptr);
    assert(str != nullptr);

    *treeError = TREE_NO_ERROR;
    Node_t *newNode = nullptr;

    #define TREE_INSERT_(branch)                                        \
        do                                                              \
        {                                                               \
        node->branch = (Node_t*)calloc(1, sizeof(Node_t));              \
        if (node->branch == nullptr)                                    \
        {                                                               \
            *treeError = TREE_INSERT_ERROR;                             \
        }                                                               \
        node->branch->elem = (char*)calloc(STR_MAX_SIZE, sizeof(char)); \
        if (node->branch->elem == nullptr)                              \
        {                                                               \
            *treeError = TREE_INSERT_ERROR;                             \
        }                                                               \
        strcpy(node->branch->elem, str);                                \
        newNode = node->branch;                                         \
        } while(0)

    if (isLeft == 1)
    {
        TREE_INSERT_(left);
    }
    else
    {
        TREE_INSERT_(right);
    }

    #undef TREE_INSERT_

    if (tree->size == 0)
    {
        Node_t *newBeginNode = tree->beginTree->left;
        free(tree->beginTree->elem);
        free(tree->beginTree);
        tree->beginTree = newBeginNode;
    }
    tree->size = tree->size + 1;
    newNode->num = tree->size + 1;

    #ifdef DEBUG
        TreeDump(tree);
    #endif // DEBUG

    return newNode;
}