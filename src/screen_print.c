#include "linus_unknow_linux.h"
#include <string.h>
#include <windows.h>

char isEmptyToken(Token* tmp); // 判断单个 token 是否为空
int opLevel(char op);

void table(int length, char left, char middle, char right) {
    putchar(left);
    for (int i = 1; i <= length - 2; i++)
        putchar(middle);
    putchar(right);
    putchar('\n');
}

/* help to choose module */
void mainMenu(void) {
    system("cls"); // depends on windows.h
    table(40, '+', '-', '+');
    printf("  1.diff(function)\n");
    printf("  2.diff(function, num)\n");
    // printf(" *3.inte(funcion)\n");
    // printf(" *4.inte(function, left, right)\n");
    printf("  5.Enter to quit\n");
    // printf("  ---\n  Those options that with '*' sign were not supported yet! T~T\n");
    printf("  ---\n  Everything is for MisakaMikoto#^v^#!\n");
    table(40, '+', '-', '+');
}

void tokenPrint(Token tokens[]) { // the max size of tokens is COMMAND_SIZE. print the tokens array like a string
    for (int i = 0; i < COMMAND_SIZE; i++) {
        if (isEmptyToken(&tokens[i]))
            break;
        if (tokens[i].isNum)
            printf("%d", tokens[i].num);
        else if (tokens[i].isOp)
            printf("%c", tokens[i].op);
        else if (tokens[i].isVar)
            printf("%c", tokens[i].var);
    }
}

void wrongPrintT(const Token* tokens, const Token* firstWrongToken, const char* printStr) {
    if (printStr != NULL)
        printf("%s\n", printStr);
    else
        putchar('\n');

    ptrdiff_t length, deltaLeft;
    deltaLeft = (firstWrongToken - tokens);
    for (int i = 0; i < COMMAND_SIZE; i++) { // find the end pointer
        if (isEmptyToken(&tokens[i])) {
            length = (&tokens[i] - &tokens[0]);
            break;
        }
    }

    table((int)length + 2, '=', '=', '=');
    putchar(' ');
    tokenPrint(tokens);
    printf("\n ");
    for (int i = 1; i <= (int)deltaLeft; i++)
        putchar('~');
    putchar('^');
    for (int i = (int)deltaLeft; i < length - 1; i++)
        putchar('~');
    putchar('\n');
    table((int)length + 2, '=', '=', '=');
}

void treePrint(Tree* root, int frameDepth) {
    if (root == NULL)
        return;

    treePrint(root->right, frameDepth + 1);

    for (int i = 0; i < frameDepth; i++)
        putchar('\t');
    if (root->isNum)
        printf("%d|(%.2lf)\n", frameDepth, root->num);
    if (root->isVar)
        printf("%d|(%c)\n", frameDepth, root->var);
    if (root->isOp)
        printf("%d|[%c]\n", frameDepth, root->op);

    treePrint(root->left, frameDepth + 1);
}

/* 传递错误字符串指针、错误字符指针，做差标识出第一个错误处 */
void wrongPrint(const char* wrongStr, const char* firstWrongCh, const char* printStr) {
    if (printStr != NULL)
        printf("%s\n", printStr);
    else
        putchar('\n');

    ptrdiff_t length = strchr(wrongStr, '\0') - wrongStr, deltaLeft = firstWrongCh - wrongStr;

    table((int)length + 2, '=', '=', '=');
    putchar(' ');
    printf(wrongStr);
    printf("\n ");
    for (int i = 1; i <= (int)deltaLeft; i++)
        putchar('~');
    putchar('^');
    for (int i = (int)deltaLeft; i < length - 1; i++)
        putchar('~');
    putchar('\n');
    table((int)length + 2, '=', '=', '=');
}

/* strcat() 函数每次都在字符串末尾（即'\0'处）追加新字符串，并且 infix 已经初始化为'\0' */
void treeToInfix(const Tree* root, char* infix, int parentLevel) {
    if (root == NULL)
        return;

    char tmp[COMMAND_SIZE / 10] = {'\0'};
    int level = root->isOp ? opLevel(root->op) : -1;
    char needBracket = ((level != -1) && (level <= parentLevel)); // compare with parent

    if (needBracket)
        strcat(infix, "(");

    /* infix */
    treeToInfix(root->left, infix, level);
    if (root->isOp) {
        if (root->op == '*')
            sprintf(tmp, ""); // 不显示乘号
        else if (root->op == '^' || root->op == '/')
            sprintf(tmp, "%c", root->op); // ^/号不分开
        else
            sprintf(tmp, " %c ", root->op);
        strcat(infix, tmp);
    }
    else if (root->isNum) {
        if (root->num == (int)(root->num)) // 如果是整数的话
            sprintf(tmp, "%d", (int)(root->num));
        else
            sprintf(tmp, "%.3lf", root->num);
        strcat(infix, tmp);
    }
    else if (root->isVar)
        strcat(infix, "x");
    treeToInfix(root->right, infix, level);

    if (needBracket)
        strcat(infix, ")");
}