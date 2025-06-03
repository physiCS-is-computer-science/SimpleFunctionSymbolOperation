#include "temp.h"
#include <stdio.h>
#include <string.h>

int infixToPostfix(const Token* token, int tokenNum, Token* postfix, const char* comm);
TreeNode* convertTree(const Token* postfix, int num, const char* comm);
void destroyTree(TreeNode* root);
void initializeToken(Token* token);
void printToken(const Token* token, int size);
int identifyToken(const char* exp, Token* token, const char* comm);

/* return mode: 0-wrong 1-non-number-mode 2-number-mode
 * 仅仅检查：命令正确、拥有左括号 的字符串。检查其括号是否闭合，如果闭合，则保证括号内有 1~2 个及以上个数逗号
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

/* 分析输入命令是哪个，返回 0 ~ 7 八种值。主函数将返回值赋给词法分析模块函数，该模块对应返回值分析 */
int formatInputCommand(char command[]) {
    char *endPtr, *enterPtr;
    ptrdiff_t defference;

    printf("\n>> ");
    fgets(command, COMMAND_SIZE, stdin);
    if (command[0] == '\n')
        return 6;

    enterPtr = strchr(command, '\n'); // because fgets() read the character '\n'
    if (enterPtr != NULL)
        *enterPtr = '\0';

    endPtr = strchr(command, '\0');
    defference = endPtr - command;
    if (defference >= COMMAND_SIZE - 1) {
        bufferClear(NULL); // 只有此时才需要 bufferClear()，否则字符串没超过最大限定长度，缓冲区没东西，bufferClear() 导致程序暂停
        printf("\nThe string length has reached the maxium allowable length(%d)", COMMAND_SIZE);
        wrongPrint(command, endPtr, NULL);
        return 0;
    }

    /* scan the string of command */
    char commandCpy[COMMAND_SIZE];
    char* tempPtr;
    int type, argMode;

    strcpy(commandCpy, command); // 不改变原参数而需要复制，前面已经保证了此处字符串可以装的下
    tempPtr = strchr(commandCpy, '('); // find command string
    if (tempPtr == NULL) {
        wrongPrint(commandCpy, commandCpy, "This string have some error:");
        return 0;
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
        wrongPrint(commandCpy, commandCpy, "This string have some error:");
        return 0;
    }
    *tempPtr = '('; // 恢复先

    switch (type) {
    case 1: // diff
        argMode = formatArgumentChar(commandCpy, tempPtr);
        if (argMode == 0)
            return 0;
        else if (argMode == 1) // non-number mode
            return 1;
        else if (argMode == 2) // number mode
            return 2;
        break;
    case 2: // inte
        argMode = formatArgumentChar(commandCpy, tempPtr);
        if (argMode == 0)
            return 0;
        else if (argMode == 1)
            return 3;
        else if (argMode == 2)
            return 4;
        break;
    case 3: // comp
        argMode = formatArgumentChar(commandCpy, tempPtr);
        if (argMode == 0)
            return 0;
        else if (argMode == 1) {
            wrongPrint(command, tempPtr + 1, "This string have some error:");
            return 0;
        }
        else if (argMode == 2)
            return 5;

        break;
    }

    printf("There are some wrongs in this string\n");
    wrongPrint(command, command, "This string have some error:");
    return 0;
}

/* 格式正确返回 1，错误返回 0 */
int formatMathArgument(const char command[], int mode) {
    TreeNode* expRoot;
    char* expStart;
    char expression[COMMAND_SIZE];

    expStart = strchr(command, '(');
    expStart++;
    if (*expStart == ',') { // this situation is like: diff(,)
        wrongPrint(command, expStart, "-=-= The expression is not exist =-=-");
        return 0;
    }
    strcpy(expression, expStart); // expStart is  the next pointer of '('
    *strchr(expression, ',') = '\0';

    /* identify the/first(mode5 have 2 expresion) expression
     * input and convert infix expression to token, and check the expression tree times */
    int expTokenNum, expPostfixNum;
    Token expToken[COMMAND_SIZE], expPostfix[COMMAND_SIZE];
    while (1) {
        initializeToken(expToken); // reset
        initializeToken(expPostfix);

        expTokenNum = identifyToken(expression, expToken, command); // convert to token and check
        if (expTokenNum == 0)
            return 0;
        printToken(expToken, expTokenNum);

        expPostfixNum = infixToPostfix(expToken, expTokenNum, expPostfix, command); // create postfix and check
        if (expPostfixNum == 0)
            return 0;
        printToken(expPostfix, expPostfixNum);

        expRoot = convertTree(expPostfix, expPostfixNum, command); // creat expression tree and check
        if (expRoot == NULL)
            return 0;

        bufferClear("Enter to close the process"); // testest
        break;
    }
    destroyTree(expRoot);

    return 1; // testest

    switch (mode) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    }
}