#include "../libs/Tree.h"
#include "../libs/FileOperations.h"

#define DEBUG

const size_t STR_MAX_SIZE = 100;
const char *TREE_GRAPH_VIZ   = "../res/graphviz.gv";
const char *INPUT_FILE_NAME  = "../res/data.txt";
const char *OUTPUT_FILE_NAME = "../res/data.txt";

static void TreeVisitPrintNodeInFile(const Node_t *node, FILE *foutput)
{
    assert(node != nullptr);
    assert(foutput != nullptr);

    fprintf(foutput, "\t%lu[shape=record, style=\"filled\", fillcolor=\"yellow\", label=\"%s\"];\n", node->num, node->elem);

    if (node->left != nullptr) TreeVisitPrintNodeInFile(node->left, foutput);
    if (node->right != nullptr) TreeVisitPrintNodeInFile(node->right, foutput);
}

static void TreeVisitPrintArrowInFile(const Node_t *node, FILE *foutput)
{
    assert(node != nullptr);
    assert(foutput != nullptr);

    if (node->left != nullptr && node->right != nullptr)
    {
        fprintf(foutput, "\t%lu -> %lu[color=\"green\", label=\"NO\", fontsize=12]\n", node->num, node->left->num);
        fprintf(foutput, "\t%lu -> %lu[color=\"blue\", label=\"YES\", fontsize=12]\n", node->num, node->right->num);
    }

    if (node->left != nullptr) TreeVisitPrintArrowInFile(node->left, foutput);
    if (node->right != nullptr) TreeVisitPrintArrowInFile(node->right, foutput);
}

void TreeDump(Tree_t *tree)
{
    FILE *graphViz = fopen(TREE_GRAPH_VIZ, "w");

    fprintf(graphViz, "digraph Tree{\n\n");
    fprintf(graphViz,"\trankdir=UD;\n\n");
    fprintf(graphViz, "\tnode[color=\"red\",fontsize=14];\n\n");

    TreeVisitPrintNodeInFile(tree->root, graphViz);

    fprintf(graphViz, "\n");

    TreeVisitPrintArrowInFile(tree->root, graphViz);

    fprintf(graphViz, "\n");

    fprintf(graphViz, "}");

    fclose(graphViz);

    system("dot -Tpng ../res/graphviz.gv -o ../res/graphviz.png");
}

TreeErrorCode TreeCtor(Tree_t *tree)
{
    assert(tree != nullptr);

    TreeErrorCode treeError = TREE_NO_ERROR;

    if (tree->status != TREE_NOT_CONSTRUCTED)
    {
        treeError = TREE_CONSTRUCTED_ERROR;
    }

    tree->root = (Node_t*)calloc(1, sizeof(Node_t));
    if (tree->root == nullptr)
    {
        treeError = TREE_CONSTRUCTED_ERROR;
    }
    tree->root->elem = (char*)calloc(STR_MAX_SIZE, sizeof(char));
    if (tree->root->elem == nullptr)
    {
        treeError = TREE_CONSTRUCTED_ERROR;
    }
    strcpy(tree->root->elem, "Do we know who?");
    tree->root->num = 1;

    tree->size = 0;
    tree->status = TREE_CONSTRUCTED;

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

    NodeDtor(tree->root);
    tree->size = 0;
    tree->size = TREE_DESTRUCTED;

    return TREE_NO_ERROR;
}

Node_t* TreeInsert(Tree_t *tree, Node_t *node, char *str, const NodeChild child, TreeErrorCode *treeError)
{
    assert(tree != nullptr);
    assert(node != nullptr);
    assert(str != nullptr);
    assert(treeError != nullptr);

    *treeError = TREE_NO_ERROR;
    Node_t *newNode = nullptr;

    #define TREE_INSERT_(branch)                                            \
        do                                                                  \
        {                                                                   \
        node->branch = (Node_t*)calloc(1, sizeof(Node_t));                  \
        if (node->branch == nullptr)                                        \
        {                                                                   \
            *treeError = TREE_INSERT_ERROR;                                 \
        }                                                                   \
        node->branch->elem = (elem_t*)calloc(STR_MAX_SIZE, sizeof(elem_t)); \
        if (node->branch->elem == nullptr)                                  \
        {                                                                   \
            *treeError = TREE_INSERT_ERROR;                                 \
        }                                                                   \
        strcpy(node->branch->elem, str);                                    \
        newNode = node->branch;                                             \
        } while(0)

    if (child == 1)
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
        Node_t *newBeginNode = tree->root->left;
        free(tree->root->elem);
        free(tree->root);
        tree->root = newBeginNode;
    }
    tree->size = tree->size + 1;
    newNode->num = tree->size + 1;

    return newNode;
}

static char* TreeReadData()
{
    FILE *finput = fopen(INPUT_FILE_NAME, "r");

    int numberBytesFile = GetFileSize(finput);
    char *str = (char*)calloc(numberBytesFile, sizeof(char));
    str = (char*)ReadFile(finput, str, numberBytesFile);

    fclose(finput);

    return str;
}

static char* StrBufferFindEndPhrase(char *str)
{
    assert(str != nullptr);

    char *firstTypeSymbol = strchr(str, '?');
    char *secondTypeSymbol = strchr(str, '!');

    #define RETURN_END_PHRASE_(num)        \
        do                                 \
        {                                  \
            *(num##TypeSymbol - 1) = '\0'; \
            return (num##TypeSymbol - 1);  \
        } while(0)

    if (firstTypeSymbol == nullptr)
    {
        RETURN_END_PHRASE_(second);
    }

    if (secondTypeSymbol == nullptr)
    {
        RETURN_END_PHRASE_(first);
    }

    if (firstTypeSymbol < secondTypeSymbol)
    {
        RETURN_END_PHRASE_(first);
    }
    else
    {
        RETURN_END_PHRASE_(second);
    }

    #undef RETURN_END_PHRASE_(num)
}

static char* StrBufferFindNewNode(char *str)
{
    assert(str != nullptr);

    while (*str != '{' && *str != '\0')
    {
        str = str + 1;
    }

    return str;
}

static char* NodeBuild(Tree_t *tree, Node_t *node, char *str, TreeErrorCode *treeError)
{
    assert(tree != nullptr);
    assert(node != nullptr);
    assert(str != nullptr);
    assert(treeError != nullptr);

    if (*str == '{')
    {
        str = str + 2;
        char number[1] = {};
        strncpy(number, str, 1);
        NodeChild child = (atoi(number) == 1) ? LEFT_CHILD : RIGHT_CHILD;
        str = str + 1;
        char *phraseEnd = StrBufferFindEndPhrase(str);
        Node_t *newNode = TreeInsert(tree, node, str, child, treeError);
        if (*(phraseEnd + 1) == '?')
        {
            str = phraseEnd + 3;
            str = NodeBuild(tree, newNode, str, treeError);
            str = NodeBuild(tree, newNode, str, treeError);
        }
        else
        {
            str = StrBufferFindNewNode(phraseEnd + 1);
        }
    }

    return str;
}

TreeErrorCode TreeBuild(Tree_t *tree)
{
    assert(tree != nullptr);

    char *str = TreeReadData();
    if (str == nullptr)
    {
        return TREE_FILL_ERROR;
    }

    char *strCopy = str;
    TreeErrorCode treeError = TREE_NO_ERROR;
    NodeBuild(tree, tree->root, strCopy, &treeError);

    TreeDump(tree);

    free(str);
    return treeError;
}

static void NodeSaveInFile(Node_t *node, FILE *foutput, NodeChild child)
{
    assert(node != nullptr);
    assert(foutput != nullptr);

    if (node->left != nullptr)
    {
        fprintf(foutput, "{ %d%s ? ", (int)child, node->elem);

        if (node->left  != nullptr) NodeSaveInFile(node->left, foutput, LEFT_CHILD);
        if (node->right != nullptr) NodeSaveInFile(node->right, foutput, RIGHT_CHILD);

        fprintf(foutput, "} ");
    }
    else
    {
        fprintf(foutput, "{ %d%s ! } ", (int)child, node->elem);
    }
}

TreeErrorCode TreeSaveInFile(Tree_t *tree)
{
    assert(tree != nullptr);

    FILE *data = fopen(OUTPUT_FILE_NAME, "w");

    NodeSaveInFile(tree->root, data, LEFT_CHILD);

    TreeDump(tree);

    fclose(data);
    return TREE_NO_ERROR;
}
