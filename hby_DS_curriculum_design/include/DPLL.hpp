#pragma once
#include "def.hpp"
#include <map>
#include <climits> // 用于INT_MAX

Status InitClauseStack(ClauseStack &CS);
Status InitLiteralStack(LiteralStack &LS);
Status CSPush(ClauseStack &CS, LinkClause p);
Status LSPush(LiteralStack &LS, LinkLiteral p);
Status CSPop(ClauseStack &CS, LinkClause &p);
Status LSPop(LiteralStack &LS, LinkLiteral &p);
int LiteralNumofClause(LinkClause cc);
int FastFindSingleClause(LinkClause C);
int JudgeClauseMapSituation(LinkClause C);
void DelClauseAndLiteral(LinkClause C, ClauseStack &Cs, LiteralStack &Ls, int num);
void RecoveryClauseAndLiteral(LinkClause C, ClauseStack &Cs, LiteralStack &Ls);
Status DPLL(LinkClause C, int strategy);

Status InitClauseStack(ClauseStack &CS)
{
    CS.base = (LinkClause *)malloc(STACK_INIT_SIZE * sizeof(LinkClause));
    if (!CS.base)
        return ERROR;
    CS.top = CS.base;
    CS.stacksize = STACK_INIT_SIZE;
    return OK;
}

Status InitLiteralStack(LiteralStack &LS)
{
    LS.base = (LinkLiteral *)malloc(STACK_INIT_SIZE * sizeof(LinkLiteral));
    if (!LS.base)
        return ERROR;
    LS.top = LS.base;
    LS.stacksize = STACK_INIT_SIZE;
    return OK;
}

Status CSPush(ClauseStack &CS, LinkClause p)
{
    if (CS.top - CS.base >= CS.stacksize)
    {
        CS.base = (LinkClause *)realloc(CS.base, (CS.stacksize + STACKINCREMENT) * sizeof(LinkClause));
        if (!CS.base)
            return ERROR;
        CS.top = CS.base + CS.stacksize;
        CS.stacksize += STACKINCREMENT;
    }
    *CS.top++ = p;
    return OK;
}

Status LSPush(LiteralStack &LS, LinkLiteral p)
{
    if (LS.top - LS.base >= LS.stacksize)
    {
        LS.base = (LinkLiteral *)realloc(LS.base, (LS.stacksize + STACKINCREMENT) * sizeof(LinkLiteral));
        if (!LS.base)
            return ERROR;
        LS.top = LS.base + LS.stacksize;
        LS.stacksize += STACKINCREMENT;
    }
    *LS.top++ = p;
    return OK;
}

Status CSPop(ClauseStack &CS, LinkClause &p)
{
    if (CS.top == CS.base)
        return ERROR;
    p = *--CS.top;
    return OK;
}

Status LSPop(LiteralStack &LS, LinkLiteral &p)
{
    if (LS.top == LS.base)
        return ERROR;
    p = *--LS.top;
    return OK;
}

int LiteralNumofClause(LinkClause cc)
{
    if (cc->state)
    {
        int n = 0;
        LinkLiteral p = cc->headpL->next;
        while (p)
        {
            if (p->state)
            {
                n++;
            }
            p = p->next;
        }
        return n;
    }
    return -1;
}

int FastFindSingleClause(LinkClause C)
{
    LinkClause p = C->next;
    while (p)
    {
        if (LiteralNumofClause(p) == 1)
        {
            LinkLiteral q = p->headpL->next;
            while (q)
            {
                if (q->state == 1)
                {
                    return q->num;
                }
                q = q->next;
            }
        }
        p = p->next;
    }
    return 0;
}

int JudgeClauseMapSituation(LinkClause C)
{
    int x = 0;
    LinkClause p = C->next;
    while (p)
    {
        if (LiteralNumofClause(p) == 0)
        {
            return 1;
        }
        if (LiteralNumofClause(p) >= 1)
        {
            x = 1;
        }
        p = p->next;
    }
    if (x)
    {
        return 2;
    }
    return 0;
}

void DelClauseAndLiteral(LinkClause C, ClauseStack &Cs, LiteralStack &Ls, int num)
{
    LiteralsInfo[abs(num)].state = 0;
    if (num > 0)
    {
        LiteralsInfo[abs(num)].value = 1;
    }
    else
    {
        LiteralsInfo[abs(num)].value = 0;
    }
    LinkClause p = C->next;
    while (p)
    {
        if (p->state)
        {
            // 如果p不为空
            LinkLiteral q1 = p->headpL->next;
            LinkLiteral q2 = p->headpL->next;
            int flag = 0;
            while (q1)
            {
                if (q1->num == num)
                {
                    p->state = 0;
                    CSPush(Cs, p);
                    flag = 1;
                    LinkLiteral t = p->headpL->next;
                    while (t)
                    {
                        LiteralsInfo[abs(t->num)].count--;
                        t = t->next;
                    }
                    break;
                }
                q1 = q1->next;
            }
            while (q2)
            {
                if (flag)
                    break;
                if (q2->num == -num)
                {
                    q2->state = 0;
                    LSPush(Ls, q2);
                }
                q2 = q2->next;
            }
        }
        p = p->next;
    }
}

void RecoveryClauseAndLiteral(ClauseStack &Cs, LiteralStack &Ls)
{
    LinkClause p;
    LinkLiteral q;
    while (CSPop(Cs, p))
    {
        p->state = 1;
        {
            LinkLiteral t = p->headpL->next;
            int num, flag = 0;
            while (t)
            {
                if (t->state == 1)
                {
                    flag++;
                    num = t->num;
                }
                LiteralsInfo[abs(t->num)].count++;
                t = t->next;
            }
            if (flag == 1)
            {
                if (num > 0)
                {
                    LiteralsInfo[num].value = 0;
                }
                else
                {
                    LiteralsInfo[abs(num)].value = 1;
                }
            }
        }
    }
    while (LSPop(Ls, q))
    {
        q->state = 1;
        int num = q->num;
        LiteralsInfo[abs(num)].state = 1;
        if (num > 0)
        {
            LiteralsInfo[num].value = 1;
        }
        else
        {
            LiteralsInfo[abs(num)].value = 0;
        }
    }
}

// 计算所有最小子句中的最大出现次数的文字
int Optimized_GetV_MOMs(LinkClause headpC)
{
    int minSize = INT_MAX;               // 初始化为最大值
    std::map<int, int> literalFrequency; // 存储每个文字在最小子句中的出现次数

    // 遍历所有子句，寻找最小子句并统计出现频率
    LinkClause pC = headpC->next;
    while (pC)
    {
        if (pC->state == 1) // 子句有效
        {
            int clauseSize = LiteralNumofClause(pC);
            if (clauseSize > 0)
            {
                if (clauseSize < minSize)
                {
                    minSize = clauseSize;     // 找到更小的子句，更新最小子句大小
                    literalFrequency.clear(); // 清空频率统计
                }
                if (clauseSize == minSize)
                {
                    // 在当前最小子句中统计文字出现频率
                    LinkLiteral pL = pC->headpL->next;
                    while (pL)
                    {
                        if (pL->state == 1)
                        {
                            literalFrequency[pL->num]++;
                        }
                        pL = pL->next;
                    }
                }
            }
        }
        pC = pC->next;
    }

    // 寻找在最小子句中出现次数最多的文字
    int selectedLiteral = 0;
    int maxFrequency = -1;
    for (auto &entry : literalFrequency)
    {
        if (entry.second > maxFrequency)
        {
            maxFrequency = entry.second;
            selectedLiteral = entry.first;
        }
    }

    return selectedLiteral;
}

int GetV1(LinkClause C)
{
    LinkClause p = C->next;
    while (p)
    {
        if (p->state == 1)
        {
            LinkLiteral q = p->headpL->next;
            while (q)
            {
                if (q->state == 1)
                {
                    return q->num;
                }
                q = q->next;
            }
        }
        p = p->next;
    }
    return 0;
}

int GetV3(void)
{
    int maxNum = 0;
    for (int i = 1; i <= n; i++)
    {
        if ((LiteralsInfo[i].state == 1) && (LiteralsInfo[maxNum].count < LiteralsInfo[i].count))
        {
            maxNum = i;
        }
    }
    return maxNum;
}

int GetV4(void)
{
    for (int i = -n; i <= n; i++)
    {
        if (i != 0)
        {
            if (LiteralsInfo[i].state == 1)
            {
                return i;
            }
        }
    }
    return 0;
}

Status DPLL(LinkClause C, int strategy)
{
    ClauseStack Cs1, Cs2;
    LiteralStack Ls1, Ls2;
    InitClauseStack(Cs1);
    InitClauseStack(Cs2);
    InitLiteralStack(Ls1);
    InitLiteralStack(Ls2);
    int num, res;
    while ((num = FastFindSingleClause(C)) != 0)
    {
        DelClauseAndLiteral(C, Cs1, Ls1, num);
    }
    res = JudgeClauseMapSituation(C);
    if (res == 0)
    {
        return TRUE;
    }
    else if (res == 1)
    {
        RecoveryClauseAndLiteral(Cs1, Ls1);
        return FALSE;
    }
    int v;
    if (strategy == 1)
    {
        v = GetV1(C);
    }
    else if (strategy == 2)
    {
        v = Optimized_GetV_MOMs(headpC);
    }
    else if (strategy == 3)
    {
        v = GetV3();
    }
    else if (strategy == 4)
    {
        v = GetV4();
    }
    int ans;
    DelClauseAndLiteral(C, Cs2, Ls2, v);
    ans = DPLL(C, strategy);
    if (ans == TRUE)
        return TRUE;
    RecoveryClauseAndLiteral(Cs2, Ls2);
    DelClauseAndLiteral(C, Cs2, Ls2, -v);
    ans = DPLL(C, strategy);
    if (ans == FALSE)
    {
        RecoveryClauseAndLiteral(Cs2, Ls2);
        RecoveryClauseAndLiteral(Cs1, Ls1);
    }
    return ans;
}

void saveRes(char filename[30], Status res, int time)
{
    char str[10];
    if (res == TRUE)
    {
        strcpy(str, "s 1\nv");
    }
    else
    {
        strcpy(str, "s 0\nv");
    }
    int i;
    FILE *fp = fopen(filename, "w");
    fputs(str, fp);
    for (i = 1; i <= n; i++)
    {
        if (LiteralsInfo[i].value == 1)
        {
            fprintf(fp, " %d", i);
        }
        else
        {
            fprintf(fp, " %d", -i);
        }
    }
    fprintf(fp, "\nt %d", time);
    fclose(fp);
}

LinkLiteral isNumInClause(int num, LinkClause pC)
{
    LinkLiteral pL = pC->headpL->next;
    while (pL)
    {
        if (pL->state)
        {
            if (pL->num == num)
            {
                return pL;
            }
        }
        pL = pL->next;
    }
    return nullptr;
}

// 利用abs(num)号变元化简ClauseMap
// 将删去的子句、文字保存,维护LiteralsInfo
void DelClauseAndLiteralforX_sudoku(LinkClause headpC, ClauseStack &CS, LiteralStack &LS, int num)
{
    LiteralsInfo[abs(num)].state = 0;
    LiteralsInfo[abs(num)].value = (num > 0 ? 1 : 0);
    LinkClause pC = headpC->next;
    while (pC)
    {
        if (pC->state)
        {
            LinkLiteral flag1 = isNumInClause(num, pC);
            LinkLiteral flag2 = isNumInClause(-num, pC);
            if (flag1)
            {
                pC->state = 0;
                CSPush(CS, pC);
                LinkLiteral pL1 = pC->headpL->next;
                while (pL1)
                {
                    if (pL1->state == 1)
                    {
                        LiteralsInfo[abs(pL1->num)].count--;
                    }
                    pL1 = pL1->next;
                }
            }
            else if (flag2)
            {
                LSPush(LS, flag2);
                flag2->state = 0;
                LiteralsInfo[abs(flag2->num)].count--;
            }
        }
        pC = pC->next;
    }
}

// 回溯
void RecoveryClauseAndLiteralforX_sudoku(ClauseStack &CS, LiteralStack &LS, int num)
{
    LiteralsInfo[abs(num)].state = 1;
    LiteralsInfo[abs(num)].value = 2;
    LinkClause pC;
    LinkLiteral pL;
    while (CSPop(CS, pC))
    {
        if (pC->state == 0)
        {
            LinkLiteral pL1 = pC->headpL->next;
            while (pL1)
            {
                if (pL1->state == 1)
                {
                    LiteralsInfo[abs(pL1->num)].count++;
                }
                pL1 = pL1->next;
            }
            pC->state = 1;
        }
    }
    while (LSPop(LS, pL))
    {
        pL->state = 1;
        LiteralsInfo[abs(pL->num)].count++;
    }
}

Status DPLLforX_sudoku(LinkClause headpC)
{
    ClauseStack CS1, CS2;
    LiteralStack LS1, LS2;
    InitClauseStack(CS1);
    InitClauseStack(CS2);
    InitLiteralStack(LS1);
    InitLiteralStack(LS2);

    int num, res;
    // 单子句传播
    while ((num = FastFindSingleClause(headpC)) != -1)
    {
        DelClauseAndLiteralforX_sudoku(headpC, CS1, LS1, num);
    }

    // 检查当前子句集的状态
    res = JudgeClauseMapSituation(headpC);
    if (res == 0) // 子句为空集
    {
        return TRUE;
    }
    else if (res == 1) // 存在空子句
    {
        RecoveryClauseAndLiteralforX_sudoku(CS1, LS1, num);
        return FALSE;
    }
    // 选择变量v
    int v = GetV1(headpC);
    Status ans;
    // 假设v为真
    DelClauseAndLiteralforX_sudoku(headpC, CS2, LS2, v);
    ans = DPLLforX_sudoku(headpC);
    if (ans == TRUE)
    {
        return TRUE;
    }
    // 回溯并假设v为假
    RecoveryClauseAndLiteralforX_sudoku(CS2, LS2, v);
    DelClauseAndLiteralforX_sudoku(headpC, CS2, LS2, -v);
    ans = DPLLforX_sudoku(headpC);
    if (ans == FALSE)
    {
        RecoveryClauseAndLiteralforX_sudoku(CS2, LS2, -v);
        RecoveryClauseAndLiteralforX_sudoku(CS1, LS1, num);
    }
    return ans;
}
