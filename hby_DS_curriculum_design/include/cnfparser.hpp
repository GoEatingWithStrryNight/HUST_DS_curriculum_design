#pragma once
#include "def.hpp"
/***********************
 * param: filename(文件名)
 * return: LinkClause
 * res: 从外界输入文件名，初始化子句邻接表以及Llinfo,并获取m,n的值，并返回邻接表头指针
 ***********************/
LinkClause InitClauseMap(char filename[])
{
    // 子句链表头指针初始化
    LinkClause C = (LinkClause)malloc(sizeof(Clause));
    if (!C)
    {
        return NULL;
    }
    C->headpL = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    char str[60], str1[50], str2[50];
    while (fgets(str, 51, fp))
    {
        if (str[0] == 'p')
        {
            sscanf(str, "%s %s %d %d", str1, str2, &n, &m);
            break;
        }
    }
    // Liinfo维护每个文字的次数和真值, n+5空间搞大一点
    if (LiteralsInfo)
        free(LiteralsInfo);
    LiteralsInfo = (LiteralInfo *)malloc((n + 5) * sizeof(LiteralInfo));
    for (int i = 0; i <= n + 3; i++)
    {
        LiteralsInfo[i].count = 0;
        LiteralsInfo[i].value = 2;
        LiteralsInfo[i].state = 1;
    }
    LinkClause p = C;
    for (int j = 0; j < m; j++)
    {
        LinkClause cc = (LinkClause)malloc(sizeof(Clause));
        cc->state = 1;
        cc->headpL = (LinkLiteral)malloc(sizeof(Literal));
        cc->headpL->next = NULL;
        cc->next = NULL;
        p->next = cc;
        p = p->next;
        char ch;
        int i = 0,
            num = 0,
            flag = 1;
        LinkLiteral ll = cc->headpL;
        fgets(str, 60, fp);
        while ((ch = str[i++]) != '\0')
        {
            if (ch == '-')
            {
                flag = -1;
            }
            else if (ch != ' ')
            {
                num = num * 10 + ch - '0';
            }
            else
            {
                LinkLiteral q = (LinkLiteral)malloc(sizeof(Literal));
                q->num = num * flag;
                q->state = 1;
                q->next = NULL;
                ll->next = q;
                ll = ll->next;
                LiteralsInfo[abs(q->num)].count++;
                num = 0;
                flag = 1;
            }
        }
    }
    fclose(fp);
    return C;
}