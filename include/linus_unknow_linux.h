/***
 * storage temprory macros and another things
 * true is TRUE_CH('t'), false is FALSE_CH */
/* -=-=-=-=-= StartTest =-=-=-=-=- */
/* -=-=-=-=-= 00EndTest =-=-=-=-=- */
#pragma once

#include <stdio.h>
#include <stdlib.h>

#define COMMAND_SIZE 200
#define TRUE_CH 't'
#define FALSE_CH '\0'
#define DIFF_STR "diff"
#define INTE_STR "inte"
#define NAN "NaN"

typedef struct BinaryTreeCell {
    char isVar, isOp, isNum, isDiff, isInte, var, op;
    double num;
    struct BinaryTreeCell* left;
    struct BinaryTreeCell* right;
} Tree;

typedef struct ExpressionTerm {
    char isVar, isOp, isNum, var, op;
    int num;
} Token;

enum CommandType {
    FALSE_INPUT,
    DIFF_CHAR,
    DIFF_NUM,
    INTE_CHAR,
    INTE_NUM
};

void bufferClear(char* screenPrint);
void wrongPrint(const char* wrongStr, const char* firstWrongCh, const char* printStr);
void wrongPrintT(const Token* tokens, const Token* firstWrongToken, const char* printStr);
