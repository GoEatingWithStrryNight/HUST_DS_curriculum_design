#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "stdbool.h"

#define Status int
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define STACK_INIT_SIZE 200
#define STACKINCREMENT 100

typedef struct Literal
/*文字定义*/
{
    int num;
    int state;
    struct Literal *next;
} Literal, *LinkLiteral;

typedef struct Clause
/*子句定义*/
{
    int state;
    LinkLiteral headpL;
    struct Clause *next;
} Clause, *LinkClause;
/****
 *子句不存在：state=0
 *空字句：state=1,包括state=1的文字个数为0
 *单字句：state=1,包括state=1的文字个数为1
 *其他情况：state=1,包括state=1的文字个数大于1
 ***/

typedef struct LiteralInfo
{
    int state; // 1存在，0不存在
    int value; // 真值 1为真，0为假，2表示不确定
    int count; // 个数
} LiteralInfo, *LinkLInfo;

typedef struct ClauseStack
{
    LinkClause *base;
    LinkClause *top;
    int stacksize;
} ClauseStack;

typedef struct LiteralStack
{
    LinkLiteral *base;
    LinkLiteral *top;
    int stacksize;
} LiteralStack;

LinkClause headpC;
LinkLInfo LiteralsInfo; // 其实更准确的命名应该是变元信息
int n, m;
