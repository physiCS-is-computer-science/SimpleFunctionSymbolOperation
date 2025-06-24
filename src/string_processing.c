/***
 * 谨以此肺腑诗歌，随我纯粹感恩之心灵，为伟大的 formatMathArgument() 函数献上！希望这是有效的排列组合于您！
 * ---
 * 《圣洁崇高之函数大乐赋》
 * 吃干营养、汗水、汗水；
 * 擎起苍穹、迈进、迈进。
 * 零壹乐谱呼啸逼近！
 * 冲烂伟大躯壳、
 * 毁灭内存宫殿。
 * 赛博蝗灾后是一片废墟，
 * 站在废墟脚下
 * 我忠心依旧。
 */

#include "linus_unknow_linux.h"
#include <string.h>

void tokenPrint(Token tokens[]); // the max size of tokens is COMMAND_SIZE
char expCorrect(char exp[]);
char convertToken(char exp[], Token tokens[]); // the size of expTokens and expression is COMMAND_SIZE
char tokenCorrect(Token tokens[]); // max size is COMMAND_SIZE
char tokenToPostfix(Token tokens[], Token postfix[]); // max: COMMAND_SIZE
Tree* postfixToTree(Token postfix[]);
void treePrint(Tree* root, int frameDepth);

/* return mode: 0-wrong 1-non-number-mode 2-number-mode*/
int formatArgumentChar(const char* commandOutput, const char* leftBracket) {
    char *firstComma = NULL, *secondComma = NULL, *endBracket = NULL;

    endBracket = strchr(leftBracket, '\0'); // 先看闭合情况
    if (*(endBracket - 1) != ')') { // if there is a ')' and it is the last one->pass
        wrongPrint(commandOutput, commandOutput, "\n<< firmatArgumentChar(): The string final character is not ')'");
        return 0;
    }

    firstComma = strchr(leftBracket, ','); // 如果到这一步，','一定在')'左边
    if (firstComma == NULL)
        return 1;
    /* 到这一行为止，找到第一个逗号 */

    secondComma = strchr(firstComma + 1, ',');
    if (secondComma == NULL) { // 无第二逗号
        if (*(firstComma + 1) == ')') { // there are not second argument in the situation
            wrongPrint(commandOutput, firstComma, "\n<< formatArgumentChar(): The second argument error");
            return 0;
        }
        else
            return 2; // diff(exp, num)
    }
    /* 到这一行为止，找到第二个逗号 */

    if (strchr(secondComma + 1, ',') != NULL) { // too many commas
        wrongPrint(commandOutput, firstComma + 1, "\n<< formatArgumentChar(): Too many commas");
        return 0;
    }

    if (*(secondComma + 1) == ')') {
        wrongPrint(commandOutput, secondComma, "\n<< formatArgumentChar(): The third argument error");
        return 0;
    }

    /* 此时：
     * 1.字符串最右端为')'
     * 2.只有两个逗号
     * 3.该逗号下一个字符不是')'
     * 即为：inte(exp, num, num) */
    return 2;
}

/* 分析输入命令是哪个，返回 6 种值。主函数将返回值赋给词法分析模块函数，该模块对应返回值分析 */
enum CommandType formatInputCommand(char command[]) {
    char *endPtr, *enterPtr; // enterPtr point to '\n' at the end of the command string
    ptrdiff_t difference;

    // printf("\n\n>> ");
    // fgets(command, COMMAND_SIZE, stdin);
    // if (command[0] == '\n') // when press key Enter
    //     return END;

    // enterPtr = strchr(command, '\n'); // because fgets() read the character '\n'
    // if (enterPtr != NULL)
    //     *enterPtr = '\0';

    endPtr = strchr(command, '\0');
    difference = endPtr - command; // length of the string
    if (difference >= COMMAND_SIZE - 1) {
        // bufferClear(NULL); // 只有此时才需要 bufferClear()，否则字符串没超过最大限定长度，缓冲区没东西，bufferClear() 导致程序暂停
        printf("\n<< formatArgumentChar(): The string length has reached the maxium allowable length(%d)", COMMAND_SIZE - 1);
        wrongPrint(command, endPtr, NULL);
        return FALSE_INPUT;
    }

    /* scan the string of command */
    char commandCpy[COMMAND_SIZE];
    char* tempPtr;
    int type;

    strcpy(commandCpy, command); // 为不改变原参数，因而需要复制，前面已经保证了此处字符串 commandCpy 可以装的下所有字符
    tempPtr = strchr(commandCpy, '('); // find command string
    if (tempPtr == NULL) {
        wrongPrint(commandCpy, commandCpy, "\n<< formatInputCommand(): Command format error");
        return FALSE_INPUT;
    }
    else
        *tempPtr = '\0'; // 此时 commandCpy 字符串被截断只剩命令关键字

    if (strcmp(DIFF_STR, commandCpy) == 0)
        type = 1;
    else if (strcmp(INTE_STR, commandCpy) == 0)
        type = 2;
    else {
        wrongPrint(commandCpy, commandCpy, "\n<< formatInputCommand(): Command error");
        return FALSE_INPUT;
    }
    *tempPtr = '('; // 恢复先

    char argMode = formatArgumentChar(commandCpy, tempPtr); // 下方 7 行已标注该函数有漏洞
    switch (type) {
    case 1: // diff
        if (argMode == 0)
            return FALSE_INPUT;
        else if (argMode == 1) // diff(exp)
            return DIFF_CHAR;
        else if (argMode == 2) // diff(exp, num)|此处有误，如果输入为：diff(exp, num1, num2)，则识别为diff(exp, num1)，错误来源于formatArgumentChar()函数
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
    }

    wrongPrint(command, command, "\n<< formatInputCommand(): This string have some error");
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
Tree* formatMathArgument(const char command[], enum CommandType type, double* x, double* right) {
    char* expStart = NULL;

    expStart = strchr(command, '(');
    expStart++; // 括号一定闭合，即一定有右括号在下一个地址，因此该指针操作无风险, expStart is the next pointer of '('
    if (*expStart == ',' || *expStart == ')') { // this situation be like: diff(,) or diff()
        wrongPrint(command, expStart, "\n<< formatMathArgument(): The expression is not exist");
        return NULL;
    }

    /* findout the expression */
    char expression[COMMAND_SIZE] = {'\0'};
    char* secondArg = NULL;
    ptrdiff_t diff;
    if (type == DIFF_CHAR || type == INTE_CHAR) { // diff(exp) or inte(exp)
        secondArg = strchr(expStart, '\0');
        secondArg--;
        diff = secondArg - expStart; // find the first ')'，diff 一定小于 COMMAND_SIZE
    }
    else if (type == DIFF_NUM || type == INTE_NUM) { // the first ',' diff(exp, ) or inte(exp, , )
        secondArg = strchr(expStart, ',');
        diff = secondArg - expStart;
    }
    // expression 数组初始化为全'\0'，因此保证 expression 数组存在 '\0' 结束字符串(strncpy()不考虑字符串末尾'\0')
    strncpy(expression, expStart, (int)diff);

    /* other paremeters proccess */
    char *secondEnd = NULL, *thirdEnd = NULL;
    double tmpX = -1, rightX = -1;
    while (*(++secondArg) == ' ') // 此时解引用应得到 '\0' 或者 ',的下一个字符'(空格跳过)
        continue;
    switch (type) { //
    case DIFF_NUM:
        tmpX = (double)strtod(secondArg, &secondEnd); // 找'导数点'
        if (secondEnd == secondArg) { //
            wrongPrint(command, secondEnd, "\n<< formatMathArgument(): Second argument error");
            return NULL;
        }
        if (*secondEnd != ')') { // DIFF_NUM模式，数字之后必为 ')'
            wrongPrint(command, secondEnd, "\n<< formatMathArgument(): This character should be ')'");
            return NULL;
        }
        *x = tmpX;
        break;
    case INTE_NUM:
        tmpX = (double)strtod(secondArg, &secondEnd); // 找积分下限
        if (secondEnd == secondArg) { //
            wrongPrint(command, secondEnd, "\n<< formatMathArgument(): Second argument error");
            return NULL;
        }
        else if (*secondEnd != ',') { // 第二个','代表第二参数结束
            wrongPrint(command, secondEnd, "\n<< formatMathArgument(): Second argumen error");
            return NULL;
        }
        *x = tmpX;
        while (*(++secondEnd) == ' ') // 此时解引用应得到 ',的下一个字符'(空格跳过)
            continue;

        rightX = (double)strtod(secondEnd, &thirdEnd); // 积分上限
        if (secondEnd == thirdEnd) {
            wrongPrint(command, thirdEnd, "\n<< formatMathArgument(): Third argument error");
            return NULL;
        }
        if (*thirdEnd != ')') {
            wrongPrint(command, thirdEnd, "\n<< formatMathArgument(): This character should be ')'");
            return NULL;
        }

        *right = rightX;
        break;
    }

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
    putchar('\n');
    /* -=-=-=-=-= 00EndTest =-=-=-=-=- */

    /* 4.tokens -> postfix */
    Token postfix[COMMAND_SIZE] = {FALSE_CH};
    if (tokenToPostfix(expTokens, postfix) == FALSE_CH)
        return NULL;
    /* -=-=-=-=-= StartTest =-=-=-=-=- */
    printf("Postfix:\n\t");
    tokenPrint(postfix);
    putchar('\n');
    /* -=-=-=-=-= 00EndTest =-=-=-=-=- */

    /* 5.postfix -> tree(~a -> (0-a)) */
    Tree* expRoot = postfixToTree(postfix);
    if (expRoot == NULL)
        return NULL;
    /* -=-=-=-=-= StartTest =-=-=-=-=- */
    printf("Expression tree:\n");
    treePrint(expRoot, 1);
    putchar('\n');
    /* -=-=-=-=-= 00EndTest =-=-=-=-=- */

    return expRoot;
}