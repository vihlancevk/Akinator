#include "Tree.h"
#include "Stack.h"
#include "UnitTests.h"

#define DEBUG

const int ANSWER_MAX_SIZE = 100;
const int STACK_SIZE = 10;

TreeErrorCode QuessingMode(Tree_t *tree);
TreeErrorCode ObjectDefinitionMode(const Tree_t *tree, const char *object);

int main()
{
    ClearLogFile();

    TreeErrorCode treeError = TREE_NO_ERROR;

    //treeError = UnitTestForTree();

    Tree_t tree = {};
    treeError = TreeCtor(&tree);

    treeError = TreeBuild(&tree);

    //treeError = QuessingMode(&tree);

    //treeError = ObjectDefinitionMode(&tree, "Андрей");

    //

    //

    treeError = TreeDtor(&tree);

    return treeError;
}

TreeErrorCode QuessingMode(Tree_t *tree)
{
    assert(tree != nullptr);

    printf("Akinator.\n");
    printf("Запущен режим угадывания объекта по признакам.\n");

    TreeErrorCode treeError = TREE_NO_ERROR;
    Node_t *node = tree->root;
    char *answer = (char*)calloc(ANSWER_MAX_SIZE, sizeof(char));

    #define READ_ANSWER_()                                     \
        do                                                     \
        {                                                      \
            memset(answer, 0, sizeof(char) * ANSWER_MAX_SIZE); \
            fgets(answer, ANSWER_MAX_SIZE, stdin);             \
            answer[strcspn(answer, "\n")] = '\0';              \
        } while(0)

    while (node->left != nullptr && node->right != nullptr)
    {
        printf("%s?\n", node->elem);
        READ_ANSWER_();
        node = ((strcmp(answer, "Нет") == 0) ? node = node->left : node = node->right);
    }

    printf("Это %s?\n", node->elem);
    READ_ANSWER_();

    if (strcmp(answer, "Нет") == 0)
    {
        printf("Кто это?\n");
        READ_ANSWER_();
        TreeInsert(tree, node, node->elem, LEFT_CHILD, &treeError);
        TreeInsert(tree, node, answer, RIGHT_CHILD, &treeError);
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
            treeError = TreeSaveInFile(tree);
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

    #undef READ_ANSWER_

    return treeError;
}

static stack_t* NodeObjectDefinition(const Node_t *node, const NodeChild child, const char *object, stack_t *stack)
{
    assert(node != nullptr);
    assert(stack != nullptr);
    assert(object != nullptr);

    char* ptrStr[1] = {};

    if (stack->size != 0)
    {
        StackPop(stack, ptrStr);
        if (strcmp(*ptrStr, object) == 0)
        {
            StackPush(stack, (stackData_t)object);
            return stack;
        }
        StackPush(stack, (stackData_t)*ptrStr);
    }

    if (node->left != nullptr)
    {
        if (node->left != nullptr)
        {
            if (child == LEFT_CHILD) StackPush(stack, (stackData_t)"не");
            StackPush(stack, node->elem);
            stack = NodeObjectDefinition(node->left, LEFT_CHILD, object, stack);
        }

        if (stack->size != 0)
        {
            StackPop(stack, ptrStr);
            if (strcmp(*ptrStr, object) == 0)
            {
                StackPush(stack, (stackData_t)object);
                return stack;
            }
            else
            {
                if (child == LEFT_CHILD) StackPop(stack, ptrStr);
            }
        }

        if (node->right != nullptr)
        {
            StackPush(stack, node->elem);
            stack = NodeObjectDefinition(node->right, RIGHT_CHILD, object, stack);
        }

    }
    else
    {
        if (strcmp(node->elem, object) == 0) StackPush(stack, node->elem);
    }

    return stack;
}

TreeErrorCode ObjectDefinitionMode(const Tree_t *tree, const char *object)
{
    assert(tree != nullptr);
    assert(object != nullptr);

    TreeErrorCode treeError = TREE_NO_ERROR;
    stack_t stack = {};
    if (STACKCTOR_(&stack, STACK_SIZE) != STACK_NO_ERROR)
    {
        treeError = TREE_OBJECT_DEFINITION_MODE_ERROR;
        return treeError;
    }

    stack = *NodeObjectDefinition(tree->root, LEFT_CHILD, object, &stack);

    char* ptrStr[1] = {};
    StackReverse(&stack);

    printf("%s : ", object);
    size_t i = stack.size;
    for (; i > 1; i--)
    {
        StackPop(&stack, ptrStr);
        if (strcmp(*ptrStr, "не") == 0)
        {
            printf("%s ", *ptrStr);
            StackPop(&stack, ptrStr);
            i--;
        }
        if (i == 3)
        {
            printf("%s и ", *ptrStr);
        }
        else if (i == 2)
        {
            printf("%s.\n", *ptrStr);
        }
        else
        {
            printf("%s, ", *ptrStr);
        }
    }

    StackDtor(&stack);
    return treeError;
}
