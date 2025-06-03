/* storage temprory macros and another things */

#pragma once

#define COMMAND_SIZE 150
#define OPERATOR "+-*/^"
#define TOKEN_SYMBOL "+-*/^()"

#define DIFF_STR "diff"
#define INTE_STR "inte"
#define COMP_STR "comp"

typedef struct BinaryTreeCell {
    char is_operator;
    char is_operand;
    char is_x;

    char operator;
    int operand;
    char x[3]; // -x\0 or x\0
    struct BinaryTreeCell* left;
    struct BinaryTreeCell* right;
} TreeNode;

typedef struct ExpressionTerm { // true is not-'\0', false is '\0'
    char isoperator;
    char isvariable;
    char isnum;

    char operator;
    char variable[3]; // -x\0 or x\0
    int num;
} Token;

void bufferClear(char* screenPrint);
void wrongPrint(const char* wrongStr, const char* firstWrongCh, const char* printStr);
