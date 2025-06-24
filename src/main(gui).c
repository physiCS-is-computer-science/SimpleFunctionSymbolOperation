/***
 * Please read the file which called README.txt
 * 0Start time: 250501
 * Finish time: 250616
 * Expectation finish time: 250629
 * By MisakaMikoto#^v^#
 *
 * Finish comment: nothing yet...
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
#include <windows.h>

Tree* formatMathArgument(const char command[], enum CommandType type, double* x, double* right); // check the argument of command, return 1/0
enum CommandType formatInputCommand(char command[]); // if the string command of inputing is format, return 1 ~ 5
Tree* copyTree(const Tree* node);
void destroyTree(Tree* root);
char treeCorrect(Tree* root);
void treePrint(Tree* root, int frameDepth);
void table(int length, char left, char middle, char right);
void diff(Tree* root);
void inte(Tree* root);
void substitutionX(Tree* root, double x); // 仅仅替换 var 为目标数字
void treeToInfix(const Tree* root, char* infix, int parentLevel);
void numSimp(Tree* root, char div); // 1
void timesOneSimp(Tree* root); // 2
void divOneSimp(Tree* root); // 3
void powOneSimp(Tree* root); // 4
void powZeroSimp(Tree* root); // 5
void addZeroSimp(Tree* root); // 6
void subZeroSimp(Tree* root); // 7

HWND hInput, hOutput, hButton;

char* calculateMain(char command[]) {
    system("cls");
    printf("-=-=-=-=-=-= Applications Log =-=-=-=-=-=-\n\n");

    char isRightTree = FALSE_CH;
    double x = -1, rightNum = -1;
    Tree* root = NULL;
    enum CommandType commandType;

    char* infix = (char*)calloc(COMMAND_SIZE, sizeof(char));

    commandType = formatInputCommand(command);
    if (commandType == FALSE_INPUT)
        return NAN;
    else if (commandType >= DIFF_CHAR && commandType <= INTE_NUM) // identify the command mode
        root = formatMathArgument(command, commandType, &x, &rightNum);
    isRightTree = treeCorrect(root);
    if (!isRightTree || root == NULL) {
        destroyTree(root);
        return NAN;
    }

    /* Compute module */
    switch (commandType) {
    case DIFF_CHAR:
        root->isDiff = TRUE_CH;
        diff(root);

        printf("---\nnumSimp(root, FALSE_CH) log:\n");
        numSimp(root, FALSE_CH); //
        treePrint(root, 1);
        break;

    case DIFF_NUM:
        root->isDiff = TRUE_CH;
        diff(root);
        substitutionX(root, x); // 带入导数点

        printf("---\nnumSimp(root, '/') log:\n");
        numSimp(root, '/'); //
        treePrint(root, 1);

        treeToInfix(root, infix, -1);

        printf("\nDestroy tree log:\n");
        destroyTree(root);
        return infix; // 无需化简直接计算数值转化字符串传递

    case INTE_CHAR:
        root->isInte = TRUE_CH;
        inte(root);

        printf("---\nnumSimp(root, FALSE_CH) log:\n");
        numSimp(root, FALSE_CH);
        treePrint(root, 1);
        break;

    case INTE_NUM:
        root->isInte = TRUE_CH;
        Tree* rootCopy = copyTree(root);
        inte(root);
        inte(rootCopy);

        substitutionX(root, rightNum); // 上限
        substitutionX(rootCopy, x); // 下限

        printf("---\nnumSimp(root, '/') log:\nthe root:\n");
        numSimp(root, '/');
        printf("the rootCopy:\n");
        numSimp(rootCopy, '/');
        sprintf(infix, "%lf", root->num - rootCopy->num);

        printf("\nDestroy tree log:\nthe root:\n");
        destroyTree(root);
        printf("the rootCopy:\n");
        destroyTree(rootCopy);
        return infix; // 直接算好了数值
    }

    printf("---\ntimesOneSimp() log:\n");
    timesOneSimp(root); // 1*a == a or a*1 == a
    treePrint(root, 1);

    printf("---\ndivOneSimp() log:\n");
    divOneSimp(root); // a/1 == a
    treePrint(root, 1);

    printf("---\npowOneSimp() log:\n");
    powOneSimp(root); // a^1 == a
    treePrint(root, 1);

    printf("---\npowZeroSimp() log:\n");
    powZeroSimp(root); // a^0 == 1
    treePrint(root, 1);

    printf("---\naddZeroSimp() log:\n");
    addZeroSimp(root); // 0+a == a or a+0 == a
    treePrint(root, 1);

    printf("---\nsubZeroSimp() log:\n");
    subZeroSimp(root); // a-0 == a
    treePrint(root, 1);

    treeToInfix(root, infix, -1);

    printf("\nDestroy tree log:\n");
    destroyTree(root);

    printf("\n-=-=-=-=-=-=-= Log End =-=-=-=-=-=-=-");

    return infix;
}

/* press button */
void OnCalculate(HWND hwnd) {
    char commandInput[COMMAND_SIZE] = {'\0'};
    GetWindowText(hInput, commandInput, sizeof(commandInput)); //

    char* output = calculateMain(commandInput);

    SetWindowText(hOutput, output);
    if (strcmp(output, NAN) != 0) // NAN不是动态分配内存，不需要释放
        free(output);
}

/* window message */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if ((HWND)lParam == hButton)
            OnCalculate(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        FillRect(ps.hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    /* high DPI */
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    SetProcessDPIAware();

    const char CLASS_NAME[] = "SymbolOperation"; // 注册窗口类

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    /* create window */
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Simple symbolic calculation",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 350,
        NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
        return 0;

    /* input text */
    hInput = CreateWindow(
        "EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        20, 20, 930, 50, // position x y and wight and hight
        hwnd, NULL, hInstance, NULL);

    /* button */
    hButton = CreateWindow(
        "BUTTON", "Calculate",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        340, 85, 300, 60, // position and size
        hwnd, NULL, hInstance, NULL);

    /* output text */
    hOutput = CreateWindow(
        "EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        20, 160, 930, 100,
        hwnd, NULL, hInstance, NULL);

    /* 字体 */
    HFONT hFont = CreateFont(32, 0, 0, 0,
                             FW_NORMAL, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                             CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                             VARIABLE_PITCH, "Consolas");

    SendMessage(hInput, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

    ShowWindow(hwnd, nCmdShow); // show window
    UpdateWindow(hwnd); // forced redraw

    /* message loop */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
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