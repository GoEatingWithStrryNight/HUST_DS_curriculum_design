#pragma once
#include "cnfparser.hpp"
#include "X_sudoku.hpp"
#include "DPLL.hpp"

void IntroductionOfX_sudoku();
void IntroductionOfX_SAT();
void str_replace(char *str1, const char *str2, const char *str3);

/****
 * 遍历一下邻接表,显示子句和文字
 * parmas:LinkClause headpC
 * return:void
 ***/
void showClauseMap(LinkClause headpC)
{
    LinkClause p = headpC->next;
    while (p)
    {
        if (p->state)
        {
            LinkLiteral pL = p->headpL->next;
            while (pL)
            {
                if (pL->state)
                {
                    printf("%d ", pL->num);
                }
                pL = pL->next;
            }
            printf("\n");
        }
        p = p->next;
    }
}

/****
 * 遍历一下LiteralsInfo
 * parmas:LinkLInfo LiteralsInfo
 * return:void
 ***/
void showLiteralsInfo(LinkLInfo LiteralsInfo)
{
    for (int i = 1; i <= n; i++)
    {
        printf("LiteralsInfor[%d].count= %d ", i, LiteralsInfo[i].count);
        printf("LiteralsInfor[%d].state= %d ", i, LiteralsInfo[i].state);
        printf("LiteralsInfor[%d].value= %d \n", i, LiteralsInfo[i].value);
    }
}

// 考虑到下面这一段要重复用,就写了一个函数来包括产生数独棋盘和答案的过程
void CreateArrAndAns(int arr[][10], int ans[][10])
{
    srand(time(NULL));
    // 初始化棋盘
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            arr[i][j] = 0;
        }
    }
    // 生成完整的对角线数独棋盘
    fillSudoku(1, 1);
    // 保存一下答案
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            ans[i][j] = arr[i][j];
        }
    }
    printf("Please choose X_sudoku diffiulty:\n");
    printf("1.Easy\n2.Medium\n3.Difficult\n");
    while (true)
    {
        scanf("%d", &difficluty_choice);
        if (difficluty_choice == 1)
        {
            holes = rand() % 11 + 30;
            break;
        }
        else if (difficluty_choice == 2)
        {
            holes = rand() % 11 + 40;
            break;
        }
        else if (difficluty_choice == 3)
        {
            holes = rand() % 11 + 50;
            break;
        }
        else
        {
            printf("Wrong Input!Please input again:");
        }
    }

    // 挖洞以创建游戏
    createPuzzle(holes);
}

/***********************
 * param: int(根据标记来判断选择哪种菜单)
 * return: void
 * res: 根据标记来判断选择哪种菜单
 ***********************/
void menu(int choice)
{
    switch (choice)
    {
    case 0:
    {
        printf("-------------------------------------------\n");
        printf("|             Initial Interface           |\n");
        printf("|                 1.X_Sudoku              |\n");
        printf("|                 2.SAT                   |\n");
        printf("|                 0.Exit                  |\n");
        printf("-------------------------------------------\n");
        break;
    }
    case 1:
    {
        printf("----------------------------------------------\n");
        printf("|                MENU of X_Sudoku            |\n");
        printf("|           1. Introduction                  |\n");
        printf("|           2. X_Sudoku Game                 |\n");
        printf("|           3.trans X_Sudoku into CnfFile    |\n");
        printf("|           4.Cnfparser and DPLL solve       |\n");
        printf("|           0. return to Initial Interface   |\n");
        printf("----------------------------------------------\n");
        break;
    }
    case 2:
    {
        printf("----------------------------------------------\n");
        printf("|               MENU of SAT                  |\n");
        printf("|           1. Introduction                  |\n");
        printf("|           2. Cnfparser and DPLL solve      |\n");
        printf("|           0. return to Initial Interface   |\n");
        printf("----------------------------------------------\n");
        break;
    }
    default:
        break;
    }
}

Status display(void)
{
    int choice = 0, branch = 0;
    while (true)
    {
        system("cls");
        menu(0);
        printf("Please input 0 or 1 or 2:");
        scanf("%d", &choice);
        if (choice == 0)
        {
            printf("Exit success!\n");
            return 0;
        }
        else if (choice == 1)
        {
            while (true)
            {
                system("cls");
                menu(1);
                printf("Please input :");
                scanf("%d", &branch);
                if (branch == 0)
                {
                    break;
                }
                else if (branch == 1)
                {
                    IntroductionOfX_sudoku();
                    system("pause");
                }
                else if (branch == 2)
                {
                    CreateArrAndAns(arr, ans);
                    int x, y, value;
                    int errorcount = 0;
                    while (true)
                    {
                        printf("\nNow shuduboard's situation is as follows:");
                        printShudu(arr);
                        printf("\nAtten: ErrorCount is %d\n", errorcount);
                        printf("input 0 0 0 you will exit this game directly\n");
                        printf("input 0 0 1 you will see the ans of this game\n");
                        printf("please input x y and value:");
                        scanf("%d %d %d", &x, &y, &value);
                        if (x == 0 && y == 0 && value == 0)
                        {
                            break;
                        }
                        else if (x == 0 && y == 0 && value == 1)
                        {
                            printf("\nThe Ans is as follows:");
                            printShudu(ans);
                            system("pause");
                            system("cls");
                            continue;
                        }
                        else if (x >= 1 && x <= 9 && y >= 1 && y <= 9 && value >= 1 && value <= 9)
                        {
                            if (ans[x][y] == value)
                            {
                                arr[x][y] = value;
                                printf("\n\n");
                                system("cls");
                                holes--;
                            }
                            else
                            {
                                errorcount++;
                                printf("this input is not right,input again!\n");
                                system("pause");
                                system("cls");
                            }
                        }
                        else
                        {
                            errorcount++;
                            printf("Unreasonable input.Please input again:\n");
                            system("pause");
                            system("cls");
                        }
                        if (errorcount == 3)
                        {
                            system("cls");
                            printf("Error 3 times.Game failure\n");
                            system("pause");
                            break;
                        }
                        if (holes == 0)
                        {
                            system("cls");
                            printf("WIN!");
                            system("pause");
                            break;
                        }
                    }
                }
                else if (branch == 3)
                {
                    CreateArrAndAns(arr, ans);
                    char X_sudokuCnfFileName[30];
                    printf("Please input the X_sudoku cnfFile name(don't include .cnf): ");
                    scanf("%s", X_sudokuCnfFileName);
                    strcat(X_sudokuCnfFileName, ".cnf");
                    printf("generate X_sudoku_board and its cnfFile now......\n");
                    CreateShuduCNF(arr, X_sudokuCnfFileName);
                    system("pause");
                    int Seechoice;
                    while (true)
                    {
                        printf("\nMUNU of SeeChoice\n1.See the X_sudoku_board\n2.See the cnf file\n0.return\n");
                        printf("Please input:");
                        scanf("%d", &Seechoice);
                        if (Seechoice == 1)
                        {
                            printShudu(arr);
                            system("pause");
                        }
                        else if (Seechoice == 2)
                        {
                            FILE *fp;
                            fp = fopen(X_sudokuCnfFileName, "r");
                            if (fp == NULL)
                            {
                                printf("Open file error!\n");
                                system("pause");
                                break;
                            }
                            char CnfFlieLineStr[50];
                            while (fgets(CnfFlieLineStr, 51, fp))
                            {
                                printf("%s", CnfFlieLineStr);
                            }
                        }
                        else if (Seechoice == 0)
                        {
                            break;
                        }
                        else
                        {
                            printf("Wrong input!Input again\n");
                        }
                    }
                }
                else if (branch == 4)
                {
                    char filename[30];
                    printf("Please input the cnfFile name(don't include .cnf): ");
                    scanf("%s", filename);
                    strcat(filename, ".cnf");
                    headpC = InitClauseMap(filename);
                    if (headpC)
                    {
                        printf("Cnfparser module work normally\n");
                        printf("check ClauseMap and LiteralsInfo :\n ");
                        showClauseMap(headpC);
                        printf("\n");
                        showLiteralsInfo(LiteralsInfo);
                    }
                    printf("Wait few seconds and input Enter\n");
                    system("pause");
                    printf("\nDPLL module is working now\n");
                    clock_t begin, end;
                    /*由于这里cnf文件对应的sudoku是有解的，所以DPLL()返回值一定是TRUE*/
                    begin = clock();
                    Status res = DPLLforX_sudoku(headpC);
                    end = clock();
                    str_replace(filename, ".cnf", ".res");
                    saveRes(filename, res, end - begin);
                    // 本地CLOCKS_PER_SEC 是 1000，则 1 个时钟计时单元对应 0.001 秒（即 1 毫秒），即end-begin的单位是毫秒
                    system("pause");
                    int DPLLans[10][10];
                    GetShuduValue(DPLLans);
                    printf("\nThe DPLL result is as follows:");
                    printShudu(DPLLans);
                    printf("\nThe actual result is as follows:");
                    printShudu(ans);
                    system("pause");
                }
                else
                {
                    printf("INPUT ERROR!Please input again\n");
                    system("pause");
                }
            }
        }
        else if (choice == 2)
        {
            while (true)
            {
                system("cls");
                menu(2);
                printf("Please input :");
                scanf("%d", &branch);
                if (branch == 0)
                {
                    break;
                }
                else if (branch == 1)
                {
                    IntroductionOfX_SAT();
                    system("pause");
                }
                else if (branch == 2)
                {
                    char filename[30];
                    printf("Please input the cnfFile name(don't include .cnf): ");
                    scanf("%s", filename);
                    strcat(filename, ".cnf");
                    headpC = InitClauseMap(filename);
                    if (headpC)
                    {
                        printf("Cnfpaeser module can work normally!\n");
                    }
                    system("pause");
                    int strategy;
                    printf("Strategy 1:choose the first v\n");
                    printf("Strategy 2:choose the most v in shortest clause\n");
                    printf("Strategy 3:choose the most v\n");
                    printf("Strategy 4:choose the first v(in LiteralsInfo)\n");
                    printf("Plaese input strategy(1 or 2 or 3 or 4):");
                    scanf("%d", &strategy);
                    printf("DPLL module is working now\n");
                    clock_t begin, end;
                    begin = clock();
                    Status res = DPLL(headpC, strategy);
                    end = clock();
                    str_replace(filename, ".cnf", ".res");
                    int cost_time = end - begin;
                    saveRes(filename, res, cost_time);
                    // 本地CLOCKS_PER_SEC 是 1000，则 1 个时钟计时单元对应 0.001 秒（即 1 毫秒），即end-begin的单位是毫秒
                    if (res)
                    {
                        printf("This SAT problem is sovleable\n");
                        printf("Time cost is %d ms\n", cost_time);
                    }
                    else
                    {
                        printf("This SAT problem is unsolveable\n");
                        printf("Time cost is %d ms\n", cost_time);
                    }
                    system("pause");
                }
                else
                {
                    printf("INPUT ERROR!Please input again\n");
                    system("pause");
                }
            }
        }
        else
        {
            printf("INPUT ERROR!Please input again\n");
            system("pause");
        }
    }
    return 0;
}

void IntroductionOfX_sudoku()
{
    printf("This interface is about X_Sudoku (2 is a separate part, 3 and 4 together are a part)\n");
    printf("Option 2: You can enter X_Sudoku game interface, there are three difficulty modes (easy, medium and hard) to play\n");
    printf("          Enter x, y, and value, and you can fill in the value in the (x, y) position until you win the game.\n");
    printf("          I provided two special inputs:0 0 0 to exit the game directly,0 0 1 to check the answer during the game.\n");
    printf("          Enjoy this game!\n");
    printf("Option 3: Similar to 2, you can generate a X_Sudoku board in difficulty mode,\n");
    printf("          program will generate the cnf file corresponding to the board.\n");
    printf("          You can choose to view the board (1) or file (2) in the SeeChoice interface.\n");
    printf("Option 4: You can input the cnf file name(generated by Option3), and program will parse the file and solve it using DPLL algorithm.\n");
    printf("          The result will be saved in a file with the same name as the res file.\n");
    printf("          And the X_sudoku answer will be presented in 9*9 board too\n");
    printf("Option 0: You can go back to the Initial Interface\n");
}

void IntroductionOfX_SAT()
{
    printf("This interface is about SAT\n");
    printf("Option 2: You can input the cnf of any type of SAT problem,and program will solve it using DPLL algorithm.\n");
    printf("          The program will tell you whether the SAT problem is sovleable.\n");
    printf("          The result will be saved in a file with the same name as the res file.\n");
    printf("Option 0: You can go back to the Initial Interface\n");
}

void str_replace(char *str1, const char *str2, const char *str3)
{
    char result[300];
    int i = 0, j = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int len3 = strlen(str3);
    int found = 0;
    while (i < len1)
    {
        //&str1[i]指的是以str[i]位置开头的指针
        if (strncmp(&str1[i], str2, len2) == 0)
        {
            strcpy(&result[j], str3);
            j += len3;
            i += len2;
            found = 1;
        }
        else
        {
            result[j++] = str1[i++];
        }
    }
    result[j] = '\0';
    if (!found)
    {
        printf("Can't find the replaced string!\n");
        return;
    }
    strcpy(str1, result);
}