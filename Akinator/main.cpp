#include "Tree.h"
#include "UnitTests.h"

#define DEBUG

const int ANSWER_MAX_SIZE = 100;

TreeErrorCode QuessingMode(Tree_t *tree);

int main()
{
    TreeErrorCode treeError = TREE_NO_ERROR;

    //treeError = UnitTestForTree();

    Tree_t tree = {};
    treeError = TreeCtor(&tree);

    treeError = TreeFill(&tree);

    treeError = QuessingMode(&tree);

    treeError = TreeDtor(&tree);

    return treeError;
}

TreeErrorCode QuessingMode(Tree_t *tree)
{
    assert(tree != nullptr);

    printf("Akinator.\n");
    printf("Запущен режим угадывания объекта по признакам.\n");

    TreeErrorCode treeError = TREE_NO_ERROR;
    Node_t *node = tree->beginTree;
    char *answer = (char*)calloc(ANSWER_MAX_SIZE, sizeof(char));

    #define READ_ANSWER_()                                     \
        do                                                     \
        {                                                      \
            memset(answer, 0, sizeof(char) * ANSWER_MAX_SIZE); \
            scanf("%s", answer);                               \
        } while(0)

    while (node->left != nullptr && node->right != nullptr)
    {
        printf("%s?\n", node->elem);
        READ_ANSWER_();

        if (strcmp(answer, "Нет") == 0)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }

    printf("Это %s?\n", node->elem);
    READ_ANSWER_();

    if (strcmp(answer, "Нет") == 0)
    {
        printf("Кто это?\n");
        READ_ANSWER_();
        TreeInsert(tree, node, node->elem, 1, &treeError);
        TreeInsert(tree, node, answer, 0, &treeError);
        printf("Чем %s отличается от %s?\n", answer, node->elem);
        READ_ANSWER_();
        strcpy(node->elem, answer);
        printf("Сохранить базу данных?\n");
        READ_ANSWER_();
        if (strcmp(answer, "Нет") == 0)
        {
            printf("База данных не сохранена!\n");
        }
        else
        {
            //ToDO
            printf("База данных сохранена!\n");
        }
    }
    else
    {
        printf("Система угадала правильный ответ!\n");
    }

    free(answer);

    #ifdef DEBUG
        TreeDump(tree);
    #endif // DEBUG

    return treeError;
}
