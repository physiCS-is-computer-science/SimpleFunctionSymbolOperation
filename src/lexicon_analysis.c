/***
 * Rules in the module of "2.string -> newString":
 * 0.number of '(' and ')' % 2
 *   if != 0 -> wP
 * 1.number of '-' % 2
 *   if it is odd: -> '-'
 *   if it is even:-> '+'
 *   if "++++...": -> '+'
 * 2.
 *   2.1.unary '-':
 *     if exp[0] == '-': -> '~'
 *     if have "/^*(" in the left of '-': -> '~'
 *   2.2.unary '+':
 *     if exp[0] == '+': -> ' '
 *     if have "/^*(" in the left of '+': -> ' '
 * 3./ 和 * 左边一定为 num，右边一定为 num 或 '~'，否则 -> wP
 *   if /0: -> wP
 * 4.遍历一遍找非"+-/^*~() 1234567890"的字符
 */

#include "temp.h"
#include <string.h>

char chPush(char stack[], char aim, int size);
char chPop(char stack[], int size);
char isChEmpty(char stack[]);

/* return NULL or pointer start */
char* isAllPlus(char* start, char* end) { // 判断是否全为 ++++++...
    if (start > end) // 确保 start 指针不大于 end 指针
        return NULL;

    if (start == end && *start == '+') // 只有一个 +
        return start;
    while (start != end) { // 此处 end 判断不到，因为到 end 就退出循环了
        if (*start != '+')
            return NULL;
        start++;
    }
    if (*end == '+') // 判断最后一个
        return start;
    else
        return NULL;
}

void opFill(char* start, char* end, char filler) { // 将这两指针之间填充为 ‘空格’ 和 ‘filler字符’，且filler在 end 处
    *end = filler;
    if (start == end) // 一个时
        return;
    while (start != end) {
        *start = ' ';
        start++;
    }
}

int minusNum(const char* start, const char* end) {
    if (start == end && *start == '-')
        return 1;

    int i = 0;
    while (start != end) {
        if (*start == '-')
            i++;
        start++;
    }
    if (*end == '-')
        return ++i;
    else
        return i;
}

char hasLeft(char exp[], char* op, char* aimStr) { // 判断 op 左边最后一个字符(空格跳过)是否是字符串 aimStr 之一
    char judge = FALSE_CH;
    char* currentCh = exp;

    for (; currentCh != op; currentCh++) {
        if (*currentCh == ' ')
            continue;
        if (strchr(aimStr, *currentCh) != NULL)
            judge = TRUE_CH;
        else
            judge = FALSE_CH;
    }

    return judge;
}

void chFind(char exp[], char* ops[], char op) {
    int i = 0;
    char* currentCh = exp;
    while (*currentCh != '\0') {
        if (*currentCh == op)
            ops[i++] = currentCh;
        currentCh++;
    }
}

/* return FALSE_CH or TRUE_CH. The function just check the string of exp out simplely */
char expCorrect(char exp[]) {
    /* 0.bracket check */
    char bracketStack[COMMAND_SIZE] = {'\0'};
    char* bracketPtr = exp;
    for (int i = 0; *bracketPtr != '\0'; i++, bracketPtr++) {
        if (*bracketPtr == '(') {
            if (chPush(bracketStack, '(', sizeof(bracketStack)) == FALSE_CH) {
                wrongPrint(exp, bracketPtr, "-=-= Error(chPush()) =-=-");
                return FALSE_CH;
            }
        }
        else if (*bracketPtr == ')') {
            if (chPop(bracketStack, sizeof(bracketStack)) == FALSE_CH) {
                wrongPrint(exp, bracketPtr, "-=-= Error(chPop()) =-=-");
                return FALSE_CH;
            }
        }
    }
    if (!isChEmpty(bracketStack)) {
        wrongPrint(exp, exp, "-=-= The bracket stack should be empty(isChEmpty()) =-=-");
        return FALSE_CH;
    }

    /* 1.-+ check */
    char door = '1';
    char *opPtr = exp, *isThat = NULL, *opStart = NULL, *opEnd = NULL;
    for (int i = 0; *opPtr != '\0'; i++, opPtr++) {
        isThat = strchr("-+", *opPtr); // 遇到 +- 时关上门开下门
        if (door == '1') {
            if (isThat) {
                opStart = opPtr;
                door = '2';
            }
        }
        else if (door == '2') {
            if (!isThat) {
                opEnd = opPtr;
                opEnd--;
                door = '3';
            }
        }

        if (door == '3') {
            if (isAllPlus(opStart, opEnd))
                opFill(opStart, opEnd, '+');
            else if (minusNum(opStart, opEnd) % 2 == 0)
                opFill(opStart, opEnd, '+');
            else if (minusNum(opStart, opEnd) % 2 == 1)
                opFill(opStart, opEnd, '-');
            door = '1';
        }
    }

    /* 2.unary -+ check */
    char *unaryMinus[COMMAND_SIZE] = {NULL}, *unaryPlus[COMMAND_SIZE] = {NULL};
    if (exp[0] == '-')
        exp[0] = '~';
    if (exp[0] == '+')
        exp[0] = ' ';

    chFind(exp, unaryMinus, '-'); // 找到所有的第三参数符号，存储其指针(位于 exp 的指针)到 unaryMinus 数组
    for (int i = 0; unaryMinus[i] != NULL; i++) {
        if (hasLeft(exp, unaryMinus[i], "/*^("))
            *unaryMinus[i] = '~';
    }
    chFind(exp, unaryPlus, '+');
    for (int i = 0; unaryPlus[i] != NULL; i++) {
        if (hasLeft(exp, unaryPlus[i], "/*^("))
            *unaryPlus[i] = ' ';
    }

    /* 3.'/' and '*' check
     * 仅仅检查这两符号左侧最后一个字符是否为 "1234567890x)" 之一(空格不算) */
    char *allDivision[COMMAND_SIZE] = {NULL}, *allTimes[COMMAND_SIZE] = {NULL};
    chFind(exp, allDivision, '/');
    chFind(exp, allTimes, '*');
    for (int i = 0; allDivision[i] != NULL; i++) {
        if (!hasLeft(exp, allDivision[i], "1234567890x)")) {
            wrongPrint(exp, allDivision[i], "-=-= Divisiion error(hasLeft()) =-=-");
            return FALSE_CH;
        }
    }
    for (int i = 0; allTimes[i] != NULL; i++) {
        if (!hasLeft(exp, allTimes[i], "1234567890x)")) {
            wrongPrint(exp, allTimes[i], "-=-= Times error(hasLeft()) =-=-");
            return FALSE_CH;
        }
    }

    /* 4.查非法字符(非"1234567890x()+-/^*~") */
    for (char* currPtr = exp; *currPtr != '\0'; currPtr++) {
        if (strchr("1234567890x()+-/^*~", *currPtr) == NULL) {
            wrongPrint(exp, currPtr, " -=-= Illegal characters =-=-");
            return FALSE_CH;
        }
    }

    return TRUE_CH;
}