#include "temp.h"
#include <stdio.h>
#include <string.h>

void destroyTree(Tree* root);

void tokenPrint(Token tokens[]); // the max size of tokens is COMMAND_SIZE
char expCorrect(char exp[]);
char convertToken(char exp[], Token tokens[]); // the size of expTokens and expression is COMMAND_SIZE
char tokenCorrect(Token tokens[]); // max size is COMMAND_SIZE
char tokenToPostfix(Token tokens[], Token postfix[]); // max: COMMAND_SIZE

/* return mode: 0-wrong 1-non-number-mode 2-number-mode
 * 仅仅检查：命令正确、拥有左括号 的字符串。
 * 并额外检查其括号是否闭合，如果闭合，则保证括号内有 1~2 个及以上个数逗号
 * 可能要重写 */
int formatArgumentChar(const char* commandOutput, const char* leftBracket) {
    char *firstComma, *secondComma, *endBracket;

    firstComma = strchr(leftBracket, ',');
    if (firstComma == NULL) {
        wrongPrint(commandOutput, leftBracket + 1, "This string have some error:");
        return 0;
    }

    secondComma = strchr(firstComma + 1, ',');
    if (secondComma == NULL) { // 此时可能是两个参数(一个逗号)
        endBracket = strchr(firstComma, ')');
        if (endBracket == NULL) {
            wrongPrint(commandOutput, firstComma + 1, "This string have some error:");
            return 0;
        }
        else {
            if (*(endBracket + 1) == '\0')
                return 1;
            else {
                wrongPrint(commandOutput, endBracket + 1, "This string have some error:");
                return 0;
            }
        }
    }
    else { // 可能是三个参数(两个逗号)
        endBracket = strchr(secondComma, ')');
        if (endBracket == NULL) {
            wrongPrint(commandOutput, secondComma + 1, "This string have some error:");
            return 0;
        }
        else {
            if (*(endBracket + 1) == '\0')
                return 2;
            else {
                wrongPrint(commandOutput, endBracket + 1, "This string have some error:");
                return 0;
            }
        }
    }
}

/* 分析输入命令是哪个，返回 0 ~ 7 八种值。主函数将返回值赋给词法分析模块函数，该模块对应返回值分析
 * function formatInputCommand() ensure the string of inputCommand[] must be like : correctCommand(,,)
 * 然而只保证括号闭合时至少 1~2 个逗号，大于2个逗号时，如果括号闭合，检测不出问题 */
enum CommandType formatInputCommand(char command[]) {
    char *endPtr, *enterPtr; // enterPtr point to '\n' at the end of the command string
    ptrdiff_t difference;

    printf("\n>> ");
    fgets(command, COMMAND_SIZE, stdin);
    if (command[0] == '\n') // 直接Enter时
        return END;

    enterPtr = strchr(command, '\n'); // because fgets() read the character '\n'
    if (enterPtr != NULL)
        *enterPtr = '\0';

    endPtr = strchr(command, '\0');
    difference = endPtr - command; // length of the string
    if (difference >= COMMAND_SIZE - 1) {
        bufferClear(NULL); // 只有此时才需要 bufferClear()，否则字符串没超过最大限定长度，缓冲区没东西，bufferClear() 导致程序暂停
        printf("\nThe string length has reached the maxium allowable length(%d)", COMMAND_SIZE - 1);
        wrongPrint(command, endPtr, NULL);
        return FALSE_INPUT;
    }

    /* scan the string of command */
    char commandCpy[COMMAND_SIZE];
    char* tempPtr;
    int type, argMode;

    strcpy(commandCpy, command); // 为不改变原参数，因而需要复制，前面已经保证了此处字符串 commandCpy 可以装的下所有字符
    tempPtr = strchr(commandCpy, '('); // find command string
    if (tempPtr == NULL) {
        wrongPrint(commandCpy, commandCpy, "-=-= Command format error =-=-");
        return FALSE_INPUT;
    }
    else
        *tempPtr = '\0'; // 此时 commandCpy 字符串被截断只剩命令关键字

    if (strcmp(DIFF_STR, commandCpy) == 0)
        type = 1;
    else if (strcmp(INTE_STR, commandCpy) == 0)
        type = 2;
    else if (strcmp(COMP_STR, commandCpy) == 0)
        type = 3;
    else {
        wrongPrint(commandCpy, commandCpy, "-=-= Command error =-=-");
        return FALSE_INPUT;
    }
    *tempPtr = '('; // 恢复先

    argMode = formatArgumentChar(commandCpy, tempPtr);
    switch (type) {
    case 1: // diff
        if (argMode == 0)
            return FALSE_INPUT;
        else if (argMode == 1) // non-number mode
            return DIFF_CHAR;
        else if (argMode == 2) // number mode
            return DIFF_NUM;
        break;
    case 2: // inte
        if (argMode == 0)
            return FALSE_INPUT;
        else if (argMode == 1)
            return INTE_CHAR;
        else if (argMode == 2)
            return INTE_NUM;
        break;
    case 3: // comp
        if (argMode == 0)
            return FALSE_INPUT;
        else if (argMode == 1) {
            wrongPrint(command, tempPtr + 1, "This string have some error:");
            return FALSE_INPUT;
        }
        else if (argMode == 2)
            return COMP;
        break;
    }

    wrongPrint(command, command, "There are some wrongs in this string");
    return FALSE_INPUT;
}

/* 格式正确返回 root，错误返回 NULL
 * 0.command string -> expression
 * 1.expression -> string
 * 2.string -> newString (rule 2)
 * 3.newString -> tokens
 * 4.tokens -> postfix
 * 5.postfix -> tree (~a -> (0-a))
 * return root of the expression tree at the end of function 'formatMathArgument()'
 * 对命令的参数进一步分析，补全 formatInputCommand() 函数的问题 */
Tree* formatMathArgument(const char command[], enum CommandType mode) {
    char* expStart;
    char expression[COMMAND_SIZE] = {'\0'};

    expStart = strchr(command, '(');
    expStart++; // 括号一定闭合，即一定有右括号在下一个地址，因此该指针操作无风险, expStart is the next pointer of '('
    if (*expStart == ',') { // this situation is like: diff(,)
        wrongPrint(command, expStart, "-=-= The expression is not exist =-=-");
        return NULL;
    }
    ptrdiff_t diff = strchr(expStart, ',') - expStart; // 找到第一个逗号，diff 一定远小于 COMMAND_SIZE
    // expression 数组初始化为全 '\0'，因此保证 expression 数组存在 '\0' 来结束字符串(strncpy函数不考虑字符串末尾 '\0')
    strncpy(expression, expStart, (int)diff);

    /* use lexicon_analysis module to identify the/first(mode5 have 2 expresion) expression
     * input and convert infix expression to token, and check the expression tree times */
    /* 2.string correct */
    if (expCorrect(expression) == FALSE_CH)
        return NULL;
    printf("String:\n\t%s\n", expression); // test

    /* 3.expression -> tokens */
    Token expTokens[COMMAND_SIZE] = {FALSE_CH}; // initialize
    if (convertToken(expression, expTokens) == FALSE_CH) //  // 确保 token 末尾有空 token 作为结束
        return NULL;
    if (tokenCorrect(expTokens) == FALSE_CH) // check again just for type of ^-
        return NULL;
    /* -=-=-=-=-= StartTest =-=-=-=-=- */
    printf("Token:\n\t");
    tokenPrint(expTokens);
    /* -=-=-=-=-= 00EndTest =-=-=-=-=- */

    /* 4.tokens -> postfix */
    Token postfix[COMMAND_SIZE] = {FALSE_CH};
    if (tokenToPostfix(expTokens, postfix) == FALSE_CH)
        return NULL;

    /* 5.postfix -> tree(~a -> (0-a)) */

    /* select mode */
    switch (mode) {
    case DIFF_CHAR:
    case DIFF_NUM:
    case INTE_CHAR:
    case INTE_NUM:
    case COMP:
    }

    /* -=-=-=-=-= StartTest =-=-=-=-=- */
    Tree* retTest;
    return retTest;
    /* -=-=-=-=-= 00EndTest =-=-=-=-=- */
}