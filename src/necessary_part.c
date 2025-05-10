/*
 * this part include some important tools
 * function _xxx() is temprory test function that will be deleted
 */

#include "temp_macro.h"
#include <stdio.h>
#include <stdlib.h>

void __process_quit_getchar(void) {
    printf("-=-= Enter to end the process =-=-");
    getchar();
    exit(0);
}

void __f5_stop_getchar(void) {
    printf("\n-=-=-= Process has ended, press key Enter to close it =-=-=-");
    getchar();
}

void __printer1(int a) {
    switch (a) {
    case 1:
        printf(" 1.%s(function, order)\n", DIFF_STR);
        break;
    case 2:
        printf(" 2.%s(function, order, num)\n", DIFF_STR);
        break;
    case 3:
        printf(" 3.%s(funcion, order)\n", INTE_STR);
        break;
    case 4:
        printf(" 4.%s(function, order, [left, right])\n", INTE_STR);
        break;
    case 5:
        printf(" 5.%s(function1, function2, operator)\n", COMP_STR);
        break;
    }
}

void buffer_clear(char* screen_print) {
    if (screen_print != NULL){
        printf("%s", screen_print);
    }
    while (getchar() != '\n')
        continue;
}