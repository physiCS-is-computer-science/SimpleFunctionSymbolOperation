/***
 * this part include some important tools
 * function __xxx() is temprory test function that will be deleted
 */

#include "linus_unknow_linux.h"
#include <string.h>
#include <stdbool.h>

void __processQuitGetchar(void) { // 临时中断程序函数
    printf("-=-= Enter to end the process(__processQuitGetchar()) =-=-");
    getchar();
    exit(0);
}

void __printer1(int a) {
    if (a == DIFF_CHAR)
        printf("1.%s(function)\n", DIFF_STR);
    if (a == DIFF_NUM)
        printf("2.%s(function, x-num)\n", DIFF_STR);
    if (a == INTE_CHAR)
        printf("3.%s(funcion)\n", INTE_STR);
    if (a == INTE_NUM)
        printf("4.%s(function, [left, right])\n", INTE_STR);
}

void bufferClear(char* screenPrint) {
    if (screenPrint != NULL)
        printf("%s", screenPrint);
    while (getchar() != '\n')
        continue;
}

/* 四种基本栈操作(char类型栈) */
char chPush(char stack[], char aim, int size) { // 成功则返回压入字符，失败返回 FALSE_CH
    int i = -1;
    while (stack[++i] != FALSE_CH) {
        if (i >= size - 1)
            return FALSE_CH;
    }
    stack[i] = aim;
    return stack[i];
}
char chPop(char stack[], int size) { // 成功则返回弹出の栈顶字符，失败返回 FALSE_CH，栈满时同样弹出栈顶
    if (stack[0] == FALSE_CH) // stack empty
        return FALSE_CH;

    char top;
    if (stack[size - 1] != FALSE_CH) { // stack full
        top = stack[size - 1];
        stack[size - 1] = FALSE_CH;
        return top;
    }

    int i = -1;
    while (stack[++i] != FALSE_CH) // normal
        continue;
    top = stack[--i];
    stack[i] = FALSE_CH;
    return top;
}
char chTop(char stack[], int size) { // 返回栈顶或 FALSE_CH
    if (stack[size - 1] != FALSE_CH) // stack full
        return stack[size - 1];

    if (stack[0] == FALSE_CH) // stack empty
        return FALSE_CH;

    int i = -1;
    while (stack[++i] != FALSE_CH)
        continue;
    return stack[--i];
}
char isChEmpty(char stack[]) { // 空栈返回 TRUE_CHAR，非空栈返回 FALSE_CH
    if (stack[0] == FALSE_CH)
        return TRUE_CH;
    else
        return FALSE_CH;
}

char isEmptyToken(Token* tmp) { // 判断单个 token 是否为空
    if (!tmp->isNum && !tmp->isOp && !tmp->isVar)
        return TRUE_CH;
    else
        return FALSE_CH;
}

/* 四种基本栈操作(存储 op 类型数据的 Token 类型栈) */
Token tokenOpPush(Token stack[], Token aim, int size) { // 返回 被压入栈的 token 或 emptyToken
    int i = -1;
    Token emptyToken = {FALSE_CH};

    while (!isEmptyToken(&stack[++i])) {
        if (i >= size - 1)
            return emptyToken;
    }
    stack[i].isOp = TRUE_CH;
    stack[i].op = aim.op;
    return stack[i];
}
Token tokenOpPop(Token stack[], int size) { // 失败返回空栈，成功弹出并返回栈顶
    Token emptyToken = {FALSE_CH};
    if (isEmptyToken(&stack[0])) // stack empty
        return emptyToken;

    Token top;
    if (!isEmptyToken(&stack[size - 1])) { // stack full
        top = stack[size - 1];
        stack[size - 1].isOp = FALSE_CH;
        stack[size - 1].op = FALSE_CH;
        return top;
    }

    int i = -1; // normal
    while (!isEmptyToken(&stack[++i]))
        continue;
    top = stack[--i];
    stack[i].isOp = FALSE_CH;
    stack[i].op = FALSE_CH;
    return top;
}
Token tokenOpTop(Token stack[], int size) {
    if (!isEmptyToken(&stack[size - 1])) // stack full
        return stack[size - 1];
    if (isEmptyToken(&stack[0])) // stack empty
        return stack[0];

    int i = -1; // normal situation
    while (!isEmptyToken(&stack[++i]))
        continue;
    return stack[--i];
}
char isTokenOpStackEmpty(Token stack[]) { // 判断 token 类型栈是否为空
    if (isEmptyToken(&stack[0]))
        return TRUE_CH;
    else
        return FALSE_CH;
}

void fillChToken(Token* aim, char ch) {
    if (strchr("x", ch) != NULL) { // 一定是x，不可能为'\0'，因为 convertToken() 调用时保证不会传递'\0'
        aim->isVar = TRUE_CH;
        aim->var = ch;
    }
    else {
        aim->isOp = TRUE_CH;
        aim->op = ch;
    }
}

void fillNumToken(Token* aim, double num) {
    aim->isNum = TRUE_CH;
    aim->num = num;
}

Token* tokenChFind(Token* tokens, char aim) { // max size is COMMAND_SIZE, 找到返回对应 token 地址，没找到返回NULL
    for (int i = 0; !isEmptyToken(&tokens[i]); i++) {
        if (tokens[i].isOp && tokens[i].op == aim)
            return &tokens[i];
        if (tokens[i].isVar && tokens[i].var == aim)
            return &tokens[i];
    }
    return NULL;
}

int opLevel(char op) {
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '~')
        return 3;
    else if (op == '^')
        return 4;
    else
        return -1;
}

void clearNode(Tree* node) { // 下面的 initNode() 和这个一样，没注意到......
    node->isDiff = FALSE_CH;
    node->isInte = FALSE_CH;
    node->isNum = FALSE_CH;
    node->isOp = FALSE_CH;
    node->isVar = FALSE_CH;
    node->num = 0;
    node->op = FALSE_CH;
    node->var = FALSE_CH;
    node->left = NULL;
    node->right = NULL;
}

/* 四种基本栈操作(Tree 类型栈) */
Tree* treePush(Tree* stack[], Tree* aim, int size) {
    int i = -1;

    while (stack[++i] != NULL) {
        if (i >= size - 1)
            return NULL;
    }
    stack[i] = aim;
    return stack[i];
}
Tree* treePop(Tree* stack[], int size) { // 失败返回NULL，成功弹出并返回栈顶
    if (stack[0] == NULL) // stack empty
        return NULL;

    Tree* top;
    if (stack[size - 1] != NULL) { // stack full
        top = stack[size - 1];
        stack[size - 1] = NULL;
        return top;
    }

    int i = -1; // normal
    while (stack[++i] != NULL)
        continue;
    top = stack[--i];
    stack[i] = NULL;
    return top;
}
Tree* treeTop(Tree* stack[], int size) { // 空栈则返回NULL
    if (stack[size - 1] != NULL) // stack full
        return stack[size - 1];
    if (stack[0] == NULL) // stack empty
        return stack[0];

    int i = -1; // normal
    while (stack[++i] != NULL)
        continue;
    return stack[--i];
}
char isTreeStackEmpty(Tree* stack[]) {
    if (stack[0] == NULL)
        return TRUE_CH;
    else
        return FALSE_CH;
}

void destroyTree(Tree* root) {
    if (root == NULL)
        return;
    destroyTree(root->left);
    destroyTree(root->right);

    /* -=-=-=-=-= StartTest =-=-=-=-=- */
    if (root->isNum)
        printf("[%.2lf]: ", root->num);
    if (root->isVar)
        printf("[%c]: ", root->var);
    if (root->isOp)
        printf("[%c]: ", root->op);
    printf("%p will be freedom\n", root);
    /* -=-=-=-=-= 00EndTest =-=-=-=-=- */

    free(root);
}

void freeStack(Tree* stack[]) {
    int i = -1;
    while (stack[++i] != NULL) {
        printf("stack[%d]-root: %p will be freedom\n", i, stack[i]); // test
        destroyTree(stack[i]);
    }
}

Tree* aNode(Token token) { // convert Token to Tree, and initialize it
    Tree* current = (Tree*)malloc(sizeof(Tree));
    clearNode(current);
    if (token.isNum) {
        current->isNum = TRUE_CH;
        current->num = token.num;
    }
    if (token.isOp) {
        current->isOp = TRUE_CH;
        current->op = token.op;
    }
    if (token.isVar) {
        current->isVar = TRUE_CH;
        current->var = token.var;
    }
    return current;
}

void initNode(Tree* node) {
    node->isDiff = FALSE_CH;
    node->isInte = FALSE_CH;
    node->isNum = FALSE_CH;
    node->isOp = FALSE_CH;
    node->isVar = FALSE_CH;
    node->num = 0;
    node->op = FALSE_CH;
    node->var = FALSE_CH;
    node->left = NULL;
    node->right = NULL;
}

Tree* copyNode(const Tree* node) { // 副本节点仅仅复制"op""num""var"，其他的都初始化为FALSE_CH（左右子指针也不复制）
    Tree* new = (Tree*)malloc(sizeof(Tree));
    initNode(new);

    if (node->isNum) {
        new->isNum = node->isNum;
        new->num = node->num;
    }
    else if (node->isOp) {
        new->isOp = node->isOp;
        new->op = node->op;
    }
    else if (node->isVar) {
        new->isVar = node->isVar;
        new->var = node->var;
    }
    return new;
}

Tree* copyTree(const Tree* node) { // 由 node 往下的树复制一份并返回根节点指针，副本树仅仅复制"op""num""var""left""right"，其他的都初始化为FALSE_CH
    if (node == NULL)
        return NULL;
    Tree* current = copyNode(node); // return the copy-pointer
    current->left = copyTree(node->left);
    current->right = copyTree(node->right);
    return current;
}

/* 该系列创建符号节点函数仅仅更改符号成员，其他成员由 initNode() 初始化为 FALSE_CH */
Tree* addNode(void) {
    Tree* node = (Tree*)malloc(sizeof(Tree));
    initNode(node);
    node->isOp == TRUE_CH;
    node->op = '+';
    return node;
}
Tree* subNode(void) {
    Tree* node = (Tree*)malloc(sizeof(Tree));
    initNode(node);
    node->isOp = TRUE_CH;
    node->op = '-';
    return node;
}
Tree* timesNode(void) {
    Tree* node = (Tree*)malloc(sizeof(Tree));
    initNode(node);
    node->isOp = TRUE_CH;
    node->op = '*';
    return node;
}
Tree* powNode(void) {
    Tree* node = (Tree*)malloc(sizeof(Tree));
    initNode(node);
    node->isOp = TRUE_CH;
    node->op = '^';
    return node;
}
Tree* one(void) { // 1 node
    Tree* node = (Tree*)malloc(sizeof(Tree));
    initNode(node);
    node->isNum = TRUE_CH;
    node->num = 1;
    return node;
}
Tree* sub01(void) { // 0 - 1 node for ~1
    Tree *zero = (Tree*)malloc(sizeof(Tree)), *tmp = subNode(), *oneNode = one();

    initNode(zero);
    zero->isNum = TRUE_CH;
    zero->num = 0;

    tmp->left = zero;
    tmp->right = oneNode;
    return tmp;
}
Tree* varNode(void){ // x
    Tree* node = (Tree*)malloc(sizeof(Tree));
    initNode(node);
    node->isVar = TRUE_CH;
    node->var = 'x';
    return node;
}

bool isNumNode(Tree* root) { // 纯数树判断
    if (root == NULL)
        return true;
    if (!(root->isNum || root->isOp))
        return false;
    bool leftJudge = isNumNode(root->left);
    bool rightJudge = isNumNode(root->right);

    return leftJudge && rightJudge; // 左右都为真方为纯数树
}