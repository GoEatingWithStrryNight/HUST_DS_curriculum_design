#pragma once
#include "def.hpp"
#include "cnfparser.hpp"
#include <vector>
#include <random>

int arr[10][10];
int ans[10][10];
int difficluty_choice, holes;
void GetShuduValue(int arr[][10]);     // 根据LiteralsInfo获取数独棋盘的值
void printShudu(int arr[][10]);        // 打印数独棋盘
int isSafe(int row, int col, int num); // 检查数字在行、列和对角线上的合法性
int fillSudoku(int row, int col);      // 生成完整数独棋盘
bool solveAndCount(int row, int col);  // 检查数独解的唯一性，通过递归求解数独来统计解的数量
bool hasUniqueSolution();
void createPuzzle(int holes);                            // 挖洞生成游戏，确保唯一解
void CreateShuduCNF(int arr[][10], char sudokuName[30]); // 创建数独的cnf公式并存入文件

/***
 * 根据LiternalsInfo得到数独棋盘的值,存入arr中
 *  1到9号文字对应[1][1]，只有一个value为1,可以得到[1][1]的值，依次类推
 ***/
void GetShuduValue(int arr[][10])
{
    int cnt = 1, i, j;
    for (i = 1; i <= 9; i++)
        for (j = 1; j <= 9; j++)
        {
            while (LiteralsInfo[cnt].value == 0)
                cnt++;
            arr[i][j] = cnt - (i - 1) * 81 - (j - 1) * 9;
            cnt++;
        }
}

// 打印数独棋盘
void printShudu(int arr[][10])
{
    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j <= 9; j++)
        {
            // if (i == 0 && j >= 1)
            //   printf("----");
            if (j == 0 && i >= 1)
                printf("|");
            if (i >= 1 && j >= 1)
            {
                if (arr[i][j] >= 1 && arr[i][j] <= 9)
                    printf(" %d |", arr[i][j]);
                else
                    printf("   |");
            }
        }
        printf("\n-------------------------------------\n");
    }
}

// 检查数字在行、列和对角线上的合法性
int isSafe(int row, int col, int num)
{
    // 检查行和列
    for (int x = 1; x <= 9; x++)
    {
        if (arr[row][x] == num || arr[x][col] == num)
        {
            return 0;
        }
    }
    // 检查3x3子宫格
    int startRow = row - (row - 1) % 3;
    int startCol = col - (col - 1) % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (arr[i + startRow][j + startCol] == num)
            {
                return 0;
            }
        }
    }
    // 检查对角线
    if (row == col)
    { // 左上到右下对角线
        for (int i = 1; i <= 9; i++)
        {
            if (arr[i][i] == num)
            {
                return 0;
            }
        }
    }
    if (row + col == 10)
    { // 右上到左下对角线
        for (int i = 1; i <= 9; i++)
        {
            if (arr[i][10 - i] == num)
            {
                return 0;
            }
        }
    }
    return 1;
}

// 生成完整数独棋盘
int fillSudoku(int row, int col)
{
    if (row == 10)
    {
        return 1;
    }
    if (col == 10)
    {
        return fillSudoku(row + 1, 1);
    }
    // 随机生成1到9的数字
    int numList[9];
    for (int i = 0; i < 9; i++)
    {
        numList[i] = i + 1;
    }
    // 洗牌把numList[0]到numList[8]（也就是1到9）打乱
    for (int i = 0; i < 9; i++)
    {
        int j = i + rand() % (9 - i);
        int temp = numList[i];
        numList[i] = numList[j];
        numList[j] = temp;
    }
    for (int i = 0; i < 9; i++)
    {
        int num = numList[i];
        if (isSafe(row, col, num))
        {
            arr[row][col] = num;
            if (fillSudoku(row, col + 1))
            {
                return 1;
            }
            arr[row][col] = 0; // 回溯
        }
    }
    return 0;
}

// 使用全局变量来计数解决方案的数量
int solutionCount = 0;

// 检查数独解的唯一性，通过递归求解数独来统计解的数量
bool solveAndCount(int row, int col)
{
    if (row == 10)
    {
        solutionCount++;
        return solutionCount == 1; // 如果有超过一个解，就停止
    }
    if (col == 10)
    {
        return solveAndCount(row + 1, 1);
    }
    if (arr[row][col] != 0)
    {
        return solveAndCount(row, col + 1);
    }
    for (int num = 1; num <= 9; num++)
    {
        if (isSafe(row, col, num))
        {
            arr[row][col] = num;
            if (!solveAndCount(row, col + 1))
            {                      // 检查解的数量
                arr[row][col] = 0; // 回溯
                return false;
            }
            arr[row][col] = 0; // 回溯
        }
    }
    return true;
}

// 验证数独是否有唯一解
bool hasUniqueSolution()
{
    solutionCount = 0; // 重置计数器
    solveAndCount(1, 1);
    return solutionCount == 1; // 唯一解则返回真
}

// 挖洞生成游戏，确保唯一解
void createPuzzle(int holes)
{
    int count = holes;
    while (count > 0)
    {
        int row = 1 + rand() % 9;
        int col = 1 + rand() % 9;
        if (arr[row][col] != 0)
        {
            int temp = arr[row][col];
            arr[row][col] = 0; // 试挖洞
            if (hasUniqueSolution())
            { // 检查唯一解
                count--;
            }
            else
            {
                arr[row][col] = temp; // 不唯一则还原
            }
        }
    }
}
/***********************
 * param: void
 * return: void
 * res:  创建数独的cnf公式并存入文件
 ***********************/
void CreateShuduCNF(int arr[][10], char sudokuName[30])
{
    int i, j, cnt = 0, hole = 0; // cnt统计子句数
    char holes[20] = "", des[30] = "p cnf 729 ";
    FILE *fp = fopen(sudokuName, "w");
    for (i = 1; i <= 9; i++)
    {
        for (j = 1; j <= 9; j++)
        {
            if (arr[i][j] == 0)
            {
                hole++;
            }
        }
    }
    itoa((12654 + 81 - hole), holes, 10);
    strcat(des, holes);
    strcat(des, "\n");
    fputs(des, fp);
    // 从另一个文件中引入数独标准SAT公式
    FILE *fp2 = fopen("C:\\Users\\lenovo\\Desktop\\hby_DS_curriculum_design\\include\\X_sudoku_base.cnf", "r");
    char str[100];
    while (fgets(str, 101, fp2) != nullptr)
    {
        fputs(str, fp);
    }
    // 对于某种特定数独的提示数
    for (i = 1; i <= 9; i++)
    {
        for (j = 1; j <= 9; j++)
        {
            if (arr[i][j] >= 1)
            {
                char str[20] = "", numStr[10];
                int num = (i - 1) * 81 + (j - 1) * 9 + arr[i][j];
                itoa(num, numStr, 10);
                strcat(str, numStr);
                strcat(str, " 0\n");
                cnt++;
                fputs(str, fp);
            }
        }
    }
    fclose(fp);
}

// 新增函数声明
std::vector<std::vector<int>> generateSudoku(int difficulty);
