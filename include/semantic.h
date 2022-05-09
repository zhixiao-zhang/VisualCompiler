#include "parser.h"
/*************Semantic Analysis***************/
//分析语法树，建立符号表
void analysis(pnode val);

//符号表结点
typedef struct var_ {
  char *name;
  char *type;
  //所在作用域是否为结构体域
  int inStruct;
  //所属的结构体编号
  int structNum;
  struct var_ *next;
} var;


void newvar(int num, ...);

int ifvardef(pnode val);

char *typevar(pnode val);

int checkleft(pnode val);

// 函数符号表的结点
typedef struct func_
{
    int tag; //0表示未定义，1表示定义
    char *name;
    char *type;
    // 是否为结构体域
    int inStruc;
    // 所属的结构体编号
    int strucNum;
    char *rtype; //声明返回值类型
    int va_num;  //记录函数形参个数
    char *va_type[10];
    struct func_ *next;
}func;
void getdetype(pnode val);//定义的参数
void getretype(pnode val);//实际的参数
void getargs(pnode Args);//获取实参
int checkrtype(pnode ID,pnode Args);//检查形参与实参是否一致
// 建立函数符号
void newfunc(int num, ...);
// 函数是否已经定义
int findfunc(pnode val);
// 函数类型
char *typefunc(pnode val);
// 函数的形参个数
int numfunc(pnode val);
// 函数实际返回值类型
void getrtype(pnode val);

// 数组符号表的结点
typedef struct array_
{
    char *name;
    char *type;
    // 是否为结构体域
    int inStruc;
    // 所属的结构体编号
    int strucNum;
    struct array_ *next;
}array;
array static *arrayhead,*arraytail;
// 建立数组符号
void newarray(int num, ...);
// 查找数组是否已经定义
int findarray(pnode val);
// 数组类型
char *typearray(pnode val);

// 结构体符号表的结点
typedef struct struc_
{
    char *name;
    char *type;
    // 是否为结构体域
    int inStruc;
    // 所属的结构体编号
    int strucNum;
    struct struc_ *next;
}struc;
struc static *struchead, *structail;
// 建立结构体符号
void newstruc(int num, ...);
// 查找结构体是否已经定义
int findstruc(pnode val);
#ifndef _SEMANTIC_
#define _SEMANTIC_
var static *varhead, *vartail;
func static *funchead,*functail;
// 记录函数实参
int static va_num;
char static * va_type[10];
char static *rtype[10];
int static rnum;
// 当前是结构体域
int static inStruc;
// 判断结构体域，{ 和 }是否抵消
int static LCnum;
// 当前是第几个结构体
int static strucNum;
#endif
