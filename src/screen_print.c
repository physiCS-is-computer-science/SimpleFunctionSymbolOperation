#include "temp.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

char isEmptyToken(Token* tmp); // 判断单个 token 是否为空

static void table(int length, char left, char middle, char right) {
    putchar(left);
    for (int i = 1; i <= length - 2; i++)
        putchar(middle);
    putchar(right);
    putchar('\n');
}

/* help to choose module */
void mainMenu(void) {
    system("cls");
    table(44, '+', '-', '+');
    printf(" 1.diff(function, order)\n");
    printf(" 2.diff(function, order, num)\n");
    printf(" 3.inte(funcion, order)\n");
    printf(" 4.inte(function, order, [left, right])\n");
    printf(" 5.comp(function1, function2, operator)\n");
    printf(" 6.Enter to quit\n");
    table(44, '+', '-', '+');
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

void wrongPrintT(const Token* tokens, const Token* firstWrongToken, const char* printStr) {
    if (printStr != NULL)
        printf("%s\n", printStr);
    else
        putchar('\n');

    ptrdiff_t length, deltaLeft;
    deltaLeft = (firstWrongToken - tokens);
    for (int i = 0; i < COMMAND_SIZE; i++) { // 找末尾指针
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