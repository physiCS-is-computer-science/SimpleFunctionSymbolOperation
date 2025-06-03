#include "temp.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

static void table(int length, char left, char middle, char right) {
    putchar(left);
    for (int i = 1; i <= length - 2; i++)
        putchar(middle);
    putchar(right);
    putchar('\n');
}

/* 传递错误字符串指针、错误字符指针，做差标识出第一个错误处 */
void wrongPrint(const char* wrongStr, const char* firstWrongCh, const char* printStr) {
    ptrdiff_t length = strchr(wrongStr, '\0') - wrongStr;
    ptrdiff_t deltaLeft = firstWrongCh - wrongStr;

    if (printStr != NULL)
        printf("%s\n", printStr);
    else
        putchar('\n');

    table((int)length + 2, '=', '=', '=');
    putchar(' ');
    printf(wrongStr);
    printf("\n ");
    for (int i = 1; i <= deltaLeft; i++)
        putchar(' ');
    putchar('^');
    for (int i = deltaLeft; i < length - 1; i++)
        putchar('~');
    putchar('\n');
    table((int)length + 2, '=', '=', '=');
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

void printToken(const Token* token, int size) {
    for (int i = 0; i < size; i++) {
        if (token[i].isnum == 't') {
            printf("'%d' ", token[i].num);
        }
        else if (token[i].isoperator == 't') {
            printf("'%c' ", token[i].operator);
        }
        else if (token[i].isvariable == 't') {
            printf("'%s' ", token[i].variable);
        }
    }
    putchar('\n');
}
