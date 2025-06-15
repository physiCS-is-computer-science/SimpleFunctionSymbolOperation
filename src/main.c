/***
 * Please read the file which called README.txt
 * 0Start time: 250501
 * Finish time: 2506
 * Expectation finish time: 250628
 * By MisakaMikoto#^v^#
 *
 * Finish comment: nothing yet.
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
void simp(Tree* root);
void diff(Tree* root);
void substitutionX(Tree* root, double x); // 仅仅替换 var 为目标数字

int main(void) {
    while (1) {
        double x = -1;
        Tree* root = NULL;
        enum CommandType commandType;

        mainMenu();

        /* string and lexical processing */
        while (root == NULL) {
            char inputCommand[COMMAND_SIZE] = {'\0'};
            commandType = formatInputCommand(inputCommand);

            if (commandType == FALSE_INPUT)
                continue;
            else if (commandType == END) // enter to quit
                return 0;
            else if (commandType >= DIFF_CHAR && commandType <= INTE_NUM) // identify the command mode
                root = formatMathArgument(inputCommand, commandType, &x);
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
        printf("Unsimplified resualt tree:\n");
        treePrint(root, 1);

        printf("\nSimplified tree log:\n");
        simp(root);
        printf("\nSimplified resualt tree:\n");
        treePrint(root, 1);

        putchar('\n');
        table(45, '#', '#', '#');
        printf("# Final resualt #\n\t\n");
        //
        table(45, '#', '#', '#');

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