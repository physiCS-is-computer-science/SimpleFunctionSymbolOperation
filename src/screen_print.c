#include <stdio.h>
#include <string.h>
#include <windows.h>

static void table(int length, char left, char middle, char right);

static void table(int length, char left, char middle, char right) {
    putchar(left);
    for (int i = 1; i <= length - 2; i++)
        putchar(middle);
    putchar(right);
    putchar('\n');
}

/* 传递错误字符串指针、错误字符指针，做差标识出第一个错误处 */
void wrong_print(const char* wrong_str, const char* first_wrong_ch) {
    ptrdiff_t length = strchr(wrong_str, '\0') - wrong_str;
    ptrdiff_t delta_left = first_wrong_ch - wrong_str;

    printf("\nThis is the first error for this string: \n");
    table((int)length + 2, '/', '=', '\\');
    putchar(' ');
    printf(wrong_str);
    printf("\n ");

    for (int i = 1; i <= delta_left; i++)
        putchar(' ');
    printf("^");
    for (int i = delta_left; i < length - 1; i++)
        putchar('~');
    printf("\n");

    table((int)length + 2, '\\', '=', '/');
}

/* help to choose module */
void main_menu(void) {
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