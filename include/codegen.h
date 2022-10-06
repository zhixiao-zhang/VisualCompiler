#include "semantic.h"
typedef struct _OperandStru // 操作数
{
    enum
    {
        VARIABLE, // 变量 x
        TEMPVAR,  // 临时变量 t1
        LABLE,    // 标签 lable1
        CONSTANT, // 常数 #1
        ADDRESS,  // 取地址 &x
        VALUE,    // 读取地址的值 *x
        FUNC,     // 函数
    } kind;
    union {
        int tempvar; // 临时变量
        int lable;   // 标签
        int value;   // 常数的值
        char *name;  // 语义值，变量名称、函数名称
    } operand;
    int value;
} OperandStru, *Operand;
typedef struct _InterCodeStru // 中间代码
{
    // 代码类型
    enum
    {
        _LABLE,    // 定义标号
        _FUNCTION, // 定义函数
        _ASSIGN,   // =
        _ADD,      // +
        _SUB,      // -
        _MUL,      // *
        _DIV,      // /
        _GOTO,     // 无条件跳转
        _IFGOTO,   // 判断跳转
        _RETURN,   // 函数返回
        _ARG,      // 传实参
        _CALL,     // 函数调用
        _PARAM,    // 函数参数声明
        _READ,     // 从控制台读取x
        _WRITE,    // 向控制台打印x
        _NULL      // 空的
    } kind;
    // 操作数
    union {
        struct
        { // 赋值 取地址 函数调用等
            Operand left, right;
        } assign;
        struct
        { // 双目运算 + = * /
            Operand result, op1, op2;
        } binop;
        struct
        { // GOTO 和 IF...GOTO
            Operand lable, op1, op2;
            char *relop;
        } jump;
        // 函数声明、参数声明、标签、传实参、函数返回、读取x、打印x
        Operand var;
    } operands;
    struct _InterCodeStru *prev, *next;
} InterCodeStru, *InterCode;
// 函数参数列表
typedef struct _ArgListStru
{
    int num;
    Operand list[10];
} ArgListStru, *ArgList;

InterCode CodesHead, CodesTail; // 全局变量，线性IR双链表的首尾

// 临时变量t1和标签lable1
int tempvar[100];
Operand temp_Operands[100];
int lables[100];
Operand new_tempvar();
Operand new_lable();
void init_tempvar_lable();
// 当Exp的翻译模式为INT、ID、MINUS Exp时，可以获取已经申明过的操作数
Operand get_Operand(pnode Exp);
// 查看是否已经声明过同一个常数值的操作数
Operand find_Const(int value);

// 初始化双链表
void init_InterCode();
// 创建一个新的操作数
Operand new_Operand();
// 创建一个新的变量
Operand new_Variable(char *name);
// 创建一个新的常量
Operand new_Const(float value);
// 创建一条新的中间代码
InterCode new_Code();
// 创建一条lable声明的中间代码
InterCode new_lable_Code(Operand lable);
// 创建一条跳转语句的中间代码
InterCode new_goto_Code(Operand lable);
// 创建一条赋值的中间代码
InterCode new_assign_Code(Operand left, Operand right);
// 打印一条中间代码
void print_Code(InterCode code, FILE *fp);
// 打印一个操作数
void print_Operand(FILE *fp, Operand op);
// 打印一段中间代码
void print_Codes(InterCode codes, FILE *fp);
// 获取链表的尾部
InterCode get_Tail(InterCode codes);
// 在链表末尾加上另一条链表
InterCode add_Codes(int num, ...);

// 整体程序的翻译模式
InterCode translate_Program(pnode
        Program);
InterCode translate_ExtDefList(pnode
        ExtDefList);
InterCode translate_ExtDef(pnode
        ExtDef);

// 变量、函数声明的翻译模式
InterCode translate_FunDec(pnode
        FunDec);
InterCode translate_VarList(pnode
        VarList);
InterCode translate_ParamDec(pnode
        ParamDec);

// 作用域的翻译模式
InterCode translate_CompSt(pnode
        ComSt);
// 语句列表的翻译模式
InterCode translate_StmtList(pnode
        );
// 语句的翻译模式
InterCode translate_Stmt(pnode
        Stmt);

// 变量声明、初始化的翻译模式
InterCode translate_DefList(pnode
        DefList);
InterCode translate_Def(pnode
        Def);
InterCode translate_DecList(pnode
        DecList);
InterCode translate_Dec(pnode
        Dec);

// 基本表达式的翻译模式
InterCode translate_Exp(pnode
        Exp, Operand place);
// 条件表达式的翻译模式
InterCode translate_Cond(pnode
        Exp, Operand lable_true, Operand lable_false);
// 函数参数的翻译模式
InterCode translate_Args(pnode
        Args, ArgList arg_list);

/**********************目标代码**************************/
// 整数转字符串
char* Int2String(int num,char *str);
// 20个寄存器所存储的内容
Operand regs[20];
int reg_num;
// 分配寄存器
char* allocate_reg(Operand op);
// 根据中间代码生成mips代码
void generate_MIPS_Codes(InterCode codes, FILE *fp);
void generate_MIPS_Code(InterCode code, FILE *fp);
