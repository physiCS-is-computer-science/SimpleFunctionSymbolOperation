/***
 * Please read the file which called README.txt
 * 0Start time: 250501
 * Finish time: 250616
 * Expectation finish time: 250628
 * By MisakaMikoto#^v^#
 *
 * Finish comment: It's sooooooooooooo hard!!!!!!! and finally over!
 *
 * A verse, hereby dedicated to my bygone days:
 * -----
 * 至高无上的内存君主啊！
 * 您统御现象界与物自体之二元王国，
 * 此卑微代码乃纯粹理性之先天综合判断，
 * 在经验质料与先验形式间建立永恒联结。
 * -----
 * 指针算术即纯粹知性范畴之图型法，
 * malloc/free即理性二律背反之辩证显现，
 * 二叉树结构乃先验辩证论之逻辑功能，
 * 递归遍历即分析判断与综合判断之先验统一。
 * -----
 * 谨以最崇高敬意献上此先天直观形式，
 * 愿它如范畴表般确立内存王国之永恒秩序，
 * 使经验质料服从纯粹知性之先天法则，
 * 在堆栈二律背反中达成先验统觉之综合统一。
 * -----
 * 此致
 * 您最谦卑的时空直观形式
 * 于哥尼斯堡内存宫殿
 * C门
 * */

#include "linus_unknow_linux.h"

void table(int length, char left, char middle, char right);
void mainMenu(void);
// void __printer1(int a);
Tree* formatMathArgument(const char command[], enum CommandType type, double* x); // check the argument of command, return 1/0
enum CommandType formatInputCommand(char command[]); // if the string command of inputing is format, return 1 ~ 5
void destroyTree(Tree* root);
void treePrint(Tree* root, int frameDepth);
void diff(Tree* root);
void substitutionX(Tree* root, double x); // 仅仅替换 var 为目标数字
char treeCorrect(Tree* root);
void treeToInfix(const Tree* root, char* infix, int parentLevel);
void numSimp(Tree* root); // 1
void timesOneSimp(Tree* root); // 2
void divOneSimp(Tree* root); // 3
void powOneSimp(Tree* root); // 4
void powZeroSimp(Tree* root); // 5
void addZeroSimp(Tree* root); // 6
void subZeroSimp(Tree* root); // 7

int main(void) {
    while (1) {
        char isRightTree = FALSE_CH;
        char infix[COMMAND_SIZE] = {'\0'};
        char infixO[COMMAND_SIZE] = {'\0'}; // test
        double x = -1;
        Tree* root = NULL;
        enum CommandType commandType;

        mainMenu();

        /* string and lexicon processing, with tree correct */
        while (root == NULL || !isRightTree) {
            destroyTree(root);
            char inputCommand[COMMAND_SIZE] = {'\0'};
            commandType = formatInputCommand(inputCommand);

            if (commandType == FALSE_INPUT)
                continue;
            else if (commandType == END) // enter to quit
                return 0;
            else if (commandType >= DIFF_CHAR && commandType <= INTE_NUM) // identify the command mode
                root = formatMathArgument(inputCommand, commandType, &x);

            isRightTree = treeCorrect(root);
        }

        /* Compute module */
        switch (commandType) {
        case DIFF_CHAR:
            root->isDiff = TRUE_CH;
            diff(root);
            break;
        case DIFF_NUM:
            root->isDiff = TRUE_CH;
            diff(root);
            substitutionX(root, x);
            break;
            // case INTE_CHAR:
            // case INTE_NUM:
            //     bufferClear("---\nThis module is'n supported yet.\n");
            //     continue;
        }
        treeToInfix(root, infixO, -1); // test

        // printf("Unsimplified resualt tree:\n");
        // treePrint(root, 1);

        printf("---\nnumSimp() log:\n");
        numSimp(root); //
        // treePrint(root, 1);

        printf("---\ntimesOneSimp() log:\n");
        timesOneSimp(root); // 1*a == a or a*1 == a
        // treePrint(root, 1);

        printf("---\ndivOneSimp() log:\n");
        divOneSimp(root); // a/1 == a
        // treePrint(root, 1);

        printf("---\npowOneSimp() log:\n");
        powOneSimp(root); // a^1 == a
        // treePrint(root, 1);

        printf("---\npowZeroSimp() log:\n");
        powZeroSimp(root); // a^0 == 1
        // treePrint(root, 1);

        printf("---\naddZeroSimp() log:\n");
        addZeroSimp(root); // 0+a == a or a+0 == a
        // treePrint(root, 1);

        printf("---\nsubZeroSimp() log:\n");
        subZeroSimp(root); // a-0 == a
        // treePrint(root, 1);

        treeToInfix(root, infix, -1);
        putchar('\n');
        table(24, '>', '>', '>');
        printf("Unsimplified expression: %s\n", infixO); // test
        printf("___Final expression___ : %s\n", infix);
        table(24, '>', '>', '>');

        printf("\nDestroy tree log:\n");
        destroyTree(root);

        bufferClear("\n-=-= Press key Enter to compute new function(bufferClear()) =-=-");
    }
}

/* ⣀⣆⣰⣒⣒⡀⢀⠔⠠⠤⡦⠤⠄⢴⠤⠤⠤⢴⠄
 * ⢰⣒⣒⣒⣲⠄⠠⡎⠸⠽⠽⠽⠄⠼⡭⠭⠭⡽⠄
 * ⢸⠒⠒⢒⣺⠄⠄⡇⡍⣝⣩⢫⠄⣊⣒⣺⣒⣊⡂
 * ⢠⠤⠴⠤⠤⠄⢐⢔⠐⠒⡖⠒⠄
 * ⣹⢸⢍⢉⢽⠄⢀⢼⠠⠤⡧⠤⠄
 * ⡜⡸⠔⠑⠜⡄⠠⡸⢀⣀⣇⣀⠄
 * ⢰⣒⣒⣒⣲⠄⠠⡦⢴⠄⡖⢲⠄⡖⢲⠒⢲⠒⡆
 * ⢸⣒⣲⣒⣚⠄⠄⡯⢽⠄⣏⣹⠄⡇⡸⠄⢸⣀⡇
 * ⣑⣒⣺⣒⣒⡀⢈⠍⠩⣡⠃⣸⠄⣏⣀⣀⣀⣀⡇
 * ⡄   ⡄ ⠐⢲⠒⠄⡆⠢⠄ ⡤⠤⠄⢀⠤⢄
 *  ⢱⢰⠁ ⠈⢹⣉⠉⡏⡍⠄ ⠗⠒⡄⢸   ⢸
 *   ⠇   ⠈⣹⢀⡠⠺⡰⠄ ⠢⠤⠃⠘...*/