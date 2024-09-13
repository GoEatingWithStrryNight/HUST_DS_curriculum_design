#include "C:\Users\lenovo\Desktop\hby_DS_curriculum_design\include\def.hpp"
#include "C:\Users\lenovo\Desktop\hby_DS_curriculum_design\include\cnfparser.hpp"
#include "C:\Users\lenovo\Desktop\hby_DS_curriculum_design\include\display.hpp"
#include "C:\Users\lenovo\Desktop\hby_DS_curriculum_design\include\DPLL.hpp"
#include "C:\Users\lenovo\Desktop\hby_DS_curriculum_design\include\X_sudoku.hpp"

int main()
{
    // 这是测试 InitClauseMap、showClauseMap、showLiteralsInfo的代码
    char filename[30];
    printf("Enter the filename: ");
    scanf("%s", filename);
    strcat(filename, ".cnf");
    LinkClause headpC = InitClauseMap(filename);
    showClauseMap(headpC);
    printf("\nn is %d m is %d\n\n", n, m);
    showLiteralsInfo(LiteralsInfo);
    return 0;
}