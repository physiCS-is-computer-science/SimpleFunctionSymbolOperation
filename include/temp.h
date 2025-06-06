/***
 * storage temprory macros and another things
 * true is TRUE_CH('t'), false is FALSE_CH
 *  */
#pragma once

#define COMMAND_SIZE 100
#define TRUE_CH 't'
#define FALSE_CH '\0'

#define DIFF_STR "diff"
#define INTE_STR "inte"
#define COMP_STR "comp"

typedef struct BinaryTreeCell {
    char isVar, isOp, isNum, var, op;
    int num;
    struct BinaryTreeCell* left;
    struct BinaryTreeCell* right;
} Tree;

typedef struct ExpressionTerm {
    char isVar, isOp, isNum, var, op;
    int num;
} Token;

void bufferClear(char* screenPrint);
void wrongPrint(const char* wrongStr, const char* firstWrongCh, const char* printStr);
