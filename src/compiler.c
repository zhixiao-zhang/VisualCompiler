#include <stdio.h>
#include <string.h>

#include "../include/codegen.h"

extern int yyparse();
extern int yyrestart(FILE *);
int i;
/*test the plug of gitglutter*/
/*test the cloud*/
/*test ctags*/

Ast newAst(char *_tokenName, int num, ...)
{
    // 生成父节点
    pnode father = (pnode)malloc(sizeof(struct parserNode));
    // 添加子节点
    pnode temp = (pnode)malloc(sizeof(struct parserNode));
    if (!father) {
        yyerror("create treenode error");
        exit(0);
    }
    father->_tokenName = _tokenName;
    father->type = NULL;

    // 参数列表，详见 stdarg.h 用法
    va_list list;
    // 初始化参数列表
    va_start(list, num);

    // 表示当前节点不是终结符号，还有子节点
    if (num > 0) {
        father->ncld = num;
        // 第一个孩子节点
        temp = va_arg(list, pnode);
        (father->cld)[0] = temp;
        Childset(temp);
        // 父节点行号为第一个孩子节点的行号
        father->line = temp->line;
        father->type = temp->type;

        if (num == 1) {
            //父节点的语义值等于左孩子的语义值
            father->content = temp->content;
            father->tag = temp->tag;
        } else {
            for (i = 1; i < num; i++) {
                temp = va_arg(list, pnode);
                (father->cld)[i] = temp;
                // 该节点为其他节点的子节点
                Childset(temp);
            }
        }
    } else  //表示当前节点是终结符（叶节点）或者空的语法单元，此时num表示行号（空单元为-1）
    {
        father->ncld = 0;
        father->line = va_arg(list, int);
        // strcmp()==0 表示相同
        if (!strcmp(_tokenName, "INT")) {
            father->type = "int";
            father->value = atoi(yytext);
        } else if (!strcmp(_tokenName, "FLOAT")) {
            father->type = "float";
            father->value = atof(yytext);
        } else {
            father->type = curType;
            // 存储词法单元语义值
            char *str;
            str = (char *)malloc(sizeof(char) * 40);
            strcpy(str, yytext);
            father->content = str;
        }
    }
    nodeList[_nodeNum] = father;
    _nodeNum++;
    return father;
}

// 父节点->左子节点->右子节点....
void Preorder(Ast ast, int level, FILE *fp)
{
    if (!strcmp(ast->_tokenName, "Program"))
        fprintf(fp, "语法分析树打印: \n");
    int i;
    if (ast != NULL) {
        // 层级结构缩进
        if (ast->line != -1) {
            // 打印节点类型
            for (i = 0; i < level; ++i) {
                fprintf(fp, " ");
            }
            fprintf(fp, "%s", ast->_tokenName);
            // 根据不同类型打印节点数据
            if ((!strcmp(ast->_tokenName, "ID")) ||
                (!strcmp(ast->_tokenName, "TYPE"))) {
                fprintf(fp, ": %s", ast->content);
            } else if (!strcmp(ast->_tokenName, "INT")) {
                fprintf(fp, ": %d", (int)(ast->value));
            } else if (!strcmp(ast->_tokenName, "FLOAT")) {
                fprintf(fp, ": %f", ast->value);
            } else {
                // 非叶节点打印行号
                fprintf(fp, "(%d)", ast->line);
            }
            fprintf(fp, "\n");
        }
        for (i = 0; i < ast->ncld; ++i) {
            Preorder((ast->cld)[i], level + 1, fp);
        }
    } else {
        return;
    }
    if (!strcmp(ast->_tokenName, "Program"))
        fprintf(fp, "打印完毕\n");
}

// 错误处理
void yyerror(char *msg)
{
    hasFault = 1;
    fprintf(stderr, "Error type B at Line %d: %s before %s\n", yylineno, msg,
            yytext);
}

// 设置节点打印状态 该节点为子节点
void Childset(pnode node)
{
    int i;
    for (i = 0; i < _nodeNum; i++) {
        if (nodeList[i] == node) {
            nodeIsChild[i] = 1;
        }
    }
}

// 先序遍历分析
void analysis(Ast ast)
{
    int i;
    if (ast != NULL) {
        for (i = 0; i < ast->ncld; ++i) {
            analysis((ast->cld)[i]);
        }
    } else
        return;
}

// 建立变量符号
void newvar(int num, ...)
{
    va_list valist;
    va_start(valist, num);

    var *res = (var *)malloc(sizeof(var));
    pnode temp = (pnode)malloc(sizeof(pnode));

    if (inStruc && LCnum) {
        // 是结构体的域
        res->inStruc = 1;
        res->structNum = strucNum;
    } else {
        res->inStruc = 0;
        res->structNum = 0;
    }

    // 变量声明 int i
    res->type = curType;
    temp = va_arg(valist, pnode);
    res->name = temp->content;

    vartail->next = res;
    vartail = res;
}
// 查找变量
int ifvardef(pnode val)
{
    var *temp = (var *)malloc(sizeof(var *));
    temp = varhead->next;
    while (temp != NULL) {
        if (!strcmp(temp->name, val->content)) {
            if (inStruc && LCnum)  // 当前变量是结构体域
            {
                if (!temp->inStruc) {
                    // 结构体域与变量重名
                    printf(
                        "Error type 9 at Line %d:Struct Field and Variable use "
                        "the same _tokenName.\n",
                        yylineno);
                } else if (temp->inStruc && temp->structNum != strucNum) {
                    // 不同结构体中的域重名
                    printf(
                        "Error type 10 at Line %d:Struct Fields use the same "
                        "_tokenName.\n",
                        yylineno);
                } else {
                    // 同一结构体中域名重复
                    return 1;
                }
            } else  // 当前变量是全局变量
            {
                if (temp->inStruc) {
                    // 变量与结构体域重名
                    printf(
                        "Error type 9 at Line %d:Struct Field and Variable use "
                        "the same _tokenName.\n",
                        yylineno);
                } else {
                    // 变量与变量重名，即重复定义
                    return 1;
                }
            }
        }
        temp = temp->next;
    }
    return 0;
}
// 变量类型
char *typevar(pnode val)
{
    var *temp = (var *)malloc(sizeof(var *));
    temp = varhead->next;
    while (temp != NULL) {
        if (!strcmp(temp->name, val->content))
            return temp->type;  //返回变量类型
        temp = temp->next;
    }
    return NULL;
}
// 赋值号左边只能出现ID、Exp LB Exp RB 以及 Exp DOT ID
int checkleft(pnode val)
{
    if (val->ncld == 1 && !strcmp((val->cld)[0]->_tokenName, "ID"))
        return 1;
    else if (val->ncld == 4 && !strcmp((val->cld)[0]->_tokenName, "Exp") &&
             !strcmp((val->cld)[1]->_tokenName, "LB") &&
             !strcmp((val->cld)[2]->_tokenName, "Exp") &&
             !strcmp((val->cld)[3]->_tokenName, "RB"))
        return 1;
    else if (val->ncld == 3 && !strcmp((val->cld)[0]->_tokenName, "Exp") &&
             !strcmp((val->cld)[1]->_tokenName, "GETMEMBER") &&
             !strcmp((val->cld)[2]->_tokenName, "ID"))
        return 1;
    else
        return 0;
}

// 创建函数符号
void newfunc(int num, ...)
{
    int i;
    va_list valist;
    va_start(valist, num);

    pnode temp = (pnode)malloc(sizeof(struct parserNode));

    switch (num) {
        case 1:
            if (inStruc && LCnum) {
                // 是结构体的域
                functail->inStruc = 1;
                functail->strucNum = strucNum;
            } else {
                functail->inStruc = 0;
                functail->strucNum = 0;
            }
            //设置函数返回值类型
            temp = va_arg(valist, pnode);
            functail->rtype = temp->content;
            functail->type = temp->type;
            for (i = 0; i < rnum; i++) {
                if (rtype[i] == NULL || strcmp(rtype[i], functail->rtype))
                    printf("Error type 12 at Line %d:Func return type error.\n",
                           yylineno);
            }
            functail->tag = 1;  //标志为已定义
            func *new = (func *)malloc(sizeof(func));
            functail->next = new;  //尾指针指向下一个空结点
            functail = new;
            break;
        case 2:
            //记录函数名
            temp = va_arg(valist, pnode);
            functail->name = temp->content;
            //设置函数声明时的参数
            temp = va_arg(valist, pnode);
            functail->va_num = 0;
            getdetype(temp);
            break;
        default:
            break;
    }
}
//定义的参数
void getdetype(pnode val)
{
    int i;
    if (val != NULL) {
        if (!strcmp(val->_tokenName, "ParamDec")) {
            functail->va_type[functail->va_num] = val->cld[0]->content;
            functail->va_num++;
            return;
        }
        for (i = 0; i < val->ncld; ++i) {
            getdetype((val->cld)[i]);
        }
    } else
        return;
}
//实际的参数
void getretype(pnode val)
{
    int i;
    if (val != NULL) {
        if (!strcmp(val->_tokenName, "Exp")) {
            va_type[va_num] = val->type;
            va_num++;
            return;
        }
        for (i = 0; i < val->ncld; ++i) {
            getretype((val->cld)[i]);
        }
    } else
        return;
}
//函数实际返回值类型
void getrtype(pnode val)
{
    rtype[rnum] = val->type;
    rnum++;
}
//检查形参与实参是否一致,没有错误返回0
int checkrtype(pnode ID, pnode Args)
{
    int i;
    va_num = 0;
    getretype(Args);
    func *temp = (func *)malloc(sizeof(func *));
    temp = funchead->next;
    while (temp != NULL && temp->name != NULL && temp->tag == 1) {
        if (!strcmp(temp->name, ID->content))
            break;
        temp = temp->next;
    }
    if (va_num != temp->va_num)
        return 1;
    for (i = 0; i < temp->va_num; i++) {
        if (temp->va_type[i] == NULL || va_type[i] == NULL ||
            strcmp(temp->va_type[i], va_type[i]) != 0)
            return 1;
    }
    return 0;
}
// 函数是否已经定义
int findfunc(pnode val)
{
    func *temp = (func *)malloc(sizeof(func *));
    temp = funchead->next;
    while (temp != NULL && temp->name != NULL && temp->tag == 1) {
        if (!strcmp(temp->name, val->content)) {
            if (inStruc && LCnum)  // 当前变量是结构体域
            {
                if (!temp->inStruc) {
                    // 结构体域与变量重名
                    printf(
                        "Error type 9 at Line %d:Struct Field and Variable use "
                        "the same _tokenName.\n",
                        yylineno);
                } else if (temp->inStruc && temp->strucNum != strucNum) {
                    // 不同结构体中的域重名
                    printf(
                        "Error type 10 at Line %d:Struct Fields use the same "
                        "_tokenName.\n",
                        yylineno);
                } else {
                    // 同一结构体中域名重复
                    return 1;
                }
            } else  // 当前变量是全局变量
            {
                if (temp->inStruc) {
                    // 变量与结构体域重名
                    printf(
                        "Error type 9 at Line %d:Struct Field and Variable use "
                        "the same _tokenName.\n",
                        yylineno);
                } else {
                    // 变量与变量重名，即重复定义
                    return 1;
                }
            }
        }
        temp = temp->next;
    }
    return 0;
}
// 函数类型
char *typefunc(pnode val)
{
    func *temp = (func *)malloc(sizeof(func *));
    temp = funchead->next;
    while (temp != NULL) {
        if (!strcmp(temp->name, val->content))
            return temp->rtype;  //返回函数类型
        temp = temp->next;
    }
    return NULL;
}
// 形参个数
int numfunc(pnode val)
{
    func *temp = (func *)malloc(sizeof(func *));
    temp = funchead->next;
    while (temp != NULL) {
        if (!strcmp(temp->name, val->content))
            return temp->va_num;  //返回形参个数
        temp = temp->next;
    }
    return 0;
}
// 创建数组符号表
void newarray(int num, ...)
{
    va_list valist;
    va_start(valist, num);

    array *res = (array *)malloc(sizeof(array));
    pnode temp = (pnode)malloc(sizeof(struct parserNode));

    if (inStruc && LCnum) {
        // 是结构体的域
        res->inStruc = 1;
        res->strucNum = strucNum;
    } else {
        res->inStruc = 0;
        res->strucNum = 0;
    }
    // int a[10]
    res->type = curType;
    temp = va_arg(valist, pnode);
    res->name = temp->content;
    arraytail->next = res;
    arraytail = res;
}
// 数组是否已经定义
int ifarraydef(pnode val)
{
    array *temp = (array *)malloc(sizeof(array *));
    temp = arrayhead->next;
    while (temp != NULL) {
        if (!strcmp(temp->name, val->content))
            return 1;
        temp = temp->next;
    }
    return 0;
}
// 数组类型
char *typearray(pnode val)
{
    array *temp = (array *)malloc(sizeof(array *));
    temp = arrayhead->next;
    while (temp != NULL) {
        if (!strcmp(temp->name, val->content))
            return temp->type;  //返回数组类型
        temp = temp->next;
    }
    return NULL;
}
// 创建结构体符号表
void newstruc(int num, ...)
{
    va_list valist;
    va_start(valist, num);

    struc *res = (struc *)malloc(sizeof(struc));
    pnode temp = (pnode)malloc(sizeof(struct parserNode));

    // struct _tokenName{}
    temp = va_arg(valist, pnode);
    res->name = temp->content;
    structail->next = res;
    structail = res;
}
// 结构体是否和结构体或变量的名字重复
int findstruc(pnode val)
{
    struc *temp = (struc *)malloc(sizeof(struc *));
    temp = struchead->next;
    while (temp != NULL) {
        if (!strcmp(temp->name, val->content))
            return 1;
        temp = temp->next;
    }
    if (ifvardef(val) == 1)
        return 1;
    return 0;
}

// 主函数 扫描文件并且分析
// 为bison会自己调用yylex()，所以在main函数中不需要再调用它了
// bison使用yyparse()进行语法分析，所以需要我们在main函数中调用yyparse()和yyrestart()
/*int main(int argc, char **argv)*/
/*{*/
/*int j, tem;*/
/*if (argc < 2) {*/
/*return 1;*/
/*}*/
/*for (i = 1; i < argc; i++) {*/
/*// 初始化，用于记录结构体域*/
/*inStruc = 0;*/
/*LCnum = 0;*/
/*strucNum = 0;*/

/*// 初始化符号表*/
/*varhead = (var *)malloc(sizeof(var));*/
/*vartail = varhead;*/
/*funchead = (func *)malloc(sizeof(func));*/
/*functail = (func *)malloc(sizeof(func));*/
/*funchead->next = functail;*/
/*functail->va_num = 0;*/
/*arrayhead = (array *)malloc(sizeof(array));*/
/*arraytail = arrayhead;*/
/*struchead = (struc *)malloc(sizeof(struc));*/
/*structail = struchead;*/
/*rnum = 0;*/

/*// 初始化节点记录列表*/
/*_nodeNum = 0;*/
/*memset(nodeList, 0, sizeof(pnode) * 10000);*/
/*memset(nodeIsChild, 0, sizeof(int) * 10000);*/
/*hasFault = 0;*/

/*FILE *f = fopen(argv[i], "r");*/
/*if (!f) {*/
/*perror(argv[i]);*/
/*return 1;*/
/*}*/
/*yyrestart(f);*/
/*yyparse();*/
/*fclose(f);*/

/*// 遍历所有非子节点的节点*/
/*if (hasFault)*/
/*continue;*/
/*for (j = 0; j < _nodeNum; j++) {*/
/*if (nodeIsChild[j] != 1) {*/
/*Preorder(nodeList[j], 0);*/
/*}*/
/*}*/
/*}*/
/*}*/

int main(int argc, char **argv)
{
    int j;
    FILE *semantic_analysis = fopen("./output/semantic_analysis.txt", "w");
    FILE *IR = fopen("./output/IR.txt", "w");
    FILE *assembly = fopen("./output/assembly.txt", "w");
    if (argc < 2) {
        return 1;
    }
    for (i = 1; i < argc; i++) {
        // 初始化，用于记录结构体域
        inStruc = 0;
        LCnum = 0;
        strucNum = 0;

        // 初始化符号表
        varhead = (var *)malloc(sizeof(var));
        vartail = varhead;
        funchead = (func *)malloc(sizeof(func));
        functail = (func *)malloc(sizeof(func));
        funchead->next = functail;
        functail->va_num = 0;
        arrayhead = (array *)malloc(sizeof(array));
        arraytail = arrayhead;
        struchead = (struc *)malloc(sizeof(struc));
        structail = struchead;
        rnum = 0;

        // 初始化节点记录列表
        _nodeNum = 0;
        memset(nodeList, 0, sizeof(pnode) * 10000);
        memset(nodeIsChild, 0, sizeof(int) * 10000);
        hasFault = 0;

        // 初始化临时变量、标签数组
        init_tempvar_lable();

        // 添加read、write函数到符号表
        char *read = "read";
        char *write = "write";
        char *typeInt = "int";
        // read函数
        functail->tag = 1;
        functail->name = read;
        functail->type = typeInt;
        functail->rtype = typeInt;
        functail->va_num = 0;
        functail->inStruc = 0;
        functail->strucNum = 0;
        // 新建节点
        func *new = (func *)malloc(sizeof(func));
        functail->next = new;  //尾指针指向下一个空结点
        functail = new;
        // write函数
        functail->tag = 1;
        functail->name = write;
        functail->va_num = 1;
        (functail->va_type)[0] = typeInt;
        functail->inStruc = 0;
        functail->strucNum = 0;
        // 新建节点
        new = (func *)malloc(sizeof(func));
        functail->next = new;  //尾指针指向下一个空结点
        functail = new;

        // 设置已经分配的寄存器数量
        reg_num = 0;

        FILE *f = fopen(argv[i], "r");
        if (!f) {
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        yyparse();
        fclose(f);

        // 遍历所有非子节点的节点
        if (hasFault)
            continue;
        for (j = 0; j < _nodeNum; j++) {
            if (nodeIsChild[j] != 1) {
                Preorder(nodeList[j], 0, semantic_analysis);
                InterCode codes = translate_Program(nodeList[j]);
                print_Codes(codes, IR);
                generate_MIPS_Codes(codes, assembly);
            }
        }
    }
}

/**********************中间代码**************************/
// 临时变量以及标签
void init_tempvar_lable()
{
    int i;
    for (i = 0; i < 100; i++) {
        // -1表示该临时变量\标签还未被使用
        tempvar[i] = -1;
        lables[i] = -1;
    }
}
Operand new_tempvar()
{
    int i;
    for (i = 0; i < 100; i++) {
        if (tempvar[i] == -1) {
            tempvar[i] = 1;
            break;
        }
    }
    if (i >= 100)
        return NULL;
    Operand result = new_Operand();
    result->kind = TEMPVAR;
    result->operand.tempvar = i + 1;
    temp_Operands[i] = result;
    return result;
}
Operand new_lable()
{
    int i;
    for (i = 0; i < 100; i++) {
        if (lables[i] == -1) {
            lables[i] = 1;
            break;
        }
    }
    if (i >= 100)
        return NULL;
    Operand result = new_Operand();
    result->kind = LABLE;
    result->operand.lable = i + 1;
    return result;
}
// 创建一个新的操作数
Operand new_Operand()
{
    Operand result = (Operand)malloc(sizeof(OperandStru));
    result->value = -10000;
    return result;
}
// 创建一个新的变量
Operand new_Variable(char *_tokenName)
{
    Operand result = new_Operand();
    result->kind = VARIABLE;
    result->operand.name = _tokenName;
    return result;
}
// 创建一个新的常量
Operand new_Const(float value)
{
    Operand result = new_Operand();
    result->kind = CONSTANT;
    result->operand.value = (int)value;
    result->value = (int)value;
    return result;
}
// 创建一条新的中间代码
InterCode new_Code()
{
    InterCode result = (InterCode)malloc(sizeof(InterCodeStru));
    result->kind = _NULL;
    result->prev = NULL;
    result->next = NULL;
    return result;
}
// 创建一条lable声明的中间代码
InterCode new_lable_Code(Operand lable)
{
    InterCode result = new_Code();
    result->kind = _LABLE;
    result->operands.var = lable;
    return result;
}
// 创建一条跳转语句的中间代码
InterCode new_goto_Code(Operand lable)
{
    InterCode result = new_Code();
    result->kind = _GOTO;
    result->operands.jump.lable = lable;
    return result;
}
// 创建一条赋值的中间代码
InterCode new_assign_Code(Operand left, Operand right)
{
    left->value = right->value;
    InterCode result = new_Code();
    result->kind = _ASSIGN;
    result->operands.assign.left = left;
    result->operands.assign.right = right;
    return result;
}
// 打印一条中间代码
void print_Code(InterCode code, FILE *fp)
{
    if (code == NULL) {
        fprintf(fp, "Error, InterCode is NULL\n");
        return;
    }
    switch (code->kind) {
        case _NULL:  // 代码为空
            // printf("Code NULL");
            break;
        case _LABLE:  // 定义标号
            fprintf(fp, "LABLE ");
            print_Operand(fp, code->operands.var);
            break;
        case _FUNCTION:  // 定义函数
            fprintf(fp, "FUNCTION ");
            print_Operand(fp, code->operands.var);
            break;
        case _ASSIGN:  // =
            print_Operand(fp, code->operands.assign.left);
            fprintf(fp, " := ");
            print_Operand(fp, code->operands.assign.right);
            break;
        case _ADD:  // +
            print_Operand(fp, code->operands.binop.result);
            fprintf(fp, " := ");
            print_Operand(fp, code->operands.binop.op1);
            fprintf(fp, " + ");
            print_Operand(fp, code->operands.binop.op2);
            break;
        case _SUB:  // -
            print_Operand(fp, code->operands.binop.result);
            fprintf(fp, " := ");
            print_Operand(fp, code->operands.binop.op1);
            fprintf(fp, " - ");
            print_Operand(fp, code->operands.binop.op2);
            break;
        case _MUL:  // *
            print_Operand(fp, code->operands.binop.result);
            fprintf(fp, " := ");
            print_Operand(fp, code->operands.binop.op1);
            fprintf(fp, " * ");
            print_Operand(fp, code->operands.binop.op2);
            break;
        case _DIV:  // /
            print_Operand(fp, code->operands.binop.result);
            fprintf(fp, " := ");
            print_Operand(fp, code->operands.binop.op1);
            fprintf(fp, " / ");
            print_Operand(fp, code->operands.binop.op2);
            break;
        case _GOTO:  // 无条件跳转
            fprintf(fp, "GOTO ");
            print_Operand(fp, code->operands.jump.lable);
            break;
        case _IFGOTO:  // 判断跳转
            fprintf(fp, "IF ");
            print_Operand(fp, code->operands.jump.op1);
            fprintf(fp, " %s ", code->operands.jump.relop);
            print_Operand(fp, code->operands.jump.op2);
            fprintf(fp, " GOTO ");
            print_Operand(fp, code->operands.jump.lable);
            break;
        case _RETURN:  // 函数返回
            fprintf(fp, "RETURN ");
            print_Operand(fp, code->operands.var);
            break;
        case _ARG:  // 传实参
            fprintf(fp, "ARG ");
            print_Operand(fp, code->operands.var);
            break;
        case _CALL:  // 函数调用
            if (code->operands.assign.left == NULL) {
                fprintf(fp, "CALL ");
            } else {
                print_Operand(fp, code->operands.assign.left);
                fprintf(fp, " := CALL ");
            }
            print_Operand(fp, code->operands.assign.right);
            break;
        case _PARAM:  // 函数参数声明
            fprintf(fp, "PARAM ");
            print_Operand(fp, code->operands.var);
            break;
        case _READ:  // 从控制台读取x
            fprintf(fp, "READ ");
            print_Operand(fp, code->operands.var);
            break;
        case _WRITE:  // 向控制台打印x
            fprintf(fp, "WRITE ");
            print_Operand(fp, code->operands.var);
            break;
        default:
            fprintf(fp, "Code Error");
            break;
    }
    if (code->kind != _NULL)
        fprintf(fp, "\n");
}
// 打印一个操作数
void print_Operand(FILE *fp, Operand op)
{
    if (op == NULL) {
        fprintf(fp, "Error, Operand is NULL\n");
        return;
    }
    switch (op->kind) {
        case VARIABLE:
        case FUNC:
            fprintf(fp, "%s", op->operand.name);
            break;
        case TEMPVAR:
            fprintf(fp, "t%d", op->operand.tempvar);
            break;
        case LABLE:
            fprintf(fp, "lable%d", op->operand.lable);
            break;
        case CONSTANT:
            fprintf(fp, "#%d", op->operand.value);
            break;
        case ADDRESS:
            fprintf(fp, "&%s", op->operand.name);
            break;
        case VALUE:
            fprintf(fp, "#%s", op->operand.name);
            break;
        default:
            fprintf(fp, "Operand Error");
            break;
    }
}
// 打印一段中间代码
void print_Codes(InterCode codes, FILE *fp)
{
    fprintf(fp, "中间代码打印：\n");
    // InterCode temp = new_Code();
    // temp = codes;
    InterCode temp = codes;
    while (temp) {
        print_Code(temp, fp);
        temp = temp->next;
    }
    fprintf(fp, "打印完毕\n");
}
// 获取链表的尾部
InterCode get_Tail(InterCode codes)
{
    InterCode temp = codes;
    while (temp->next) {
        temp = temp->next;
    }
    return temp;
}
// 在链表末尾加上另一条链表
InterCode add_Codes(int num, ...)
{
    int i;
    // 参数列表，详见 stdarg.h 用法
    va_list list;
    // 初始化参数列表
    va_start(list, num);
    // 拼接中间代码
    InterCode code = va_arg(list, InterCode);
    InterCode temp = new_Code();
    InterCode tail = new_Code();
    for (i = 1; i < num; i++) {
        temp = va_arg(list, InterCode);
        tail = get_Tail(code);
        tail->next = temp;
        temp->prev = tail;
    }
    return code;
}

// 整体程序的翻译模式
InterCode translate_Program(pnode Program)
{
    // ExtDefList
    if (Program->ncld == 1) {
        return translate_ExtDefList((Program->cld)[0]);
    }
    return new_Code();
}
InterCode translate_ExtDefList(pnode ExtDefList)
{
    // ExtDef ExtDefList
    if (ExtDefList->ncld == 2) {
        InterCode code1 = translate_ExtDef((ExtDefList->cld)[0]);
        InterCode code2 = translate_ExtDefList((ExtDefList->cld)[1]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
InterCode translate_ExtDef(pnode ExtDef)
{
    // Specifire ExtDecList SEMI
    if (ExtDef->ncld == 3 &&
        !strcmp((ExtDef->cld)[1]->_tokenName, "ExtDecList")) {
        // 函数外变量声明、结构体定义不进行处理
    }
    // Specifire SEMI
    else if (ExtDef->ncld == 2 &&
             !strcmp((ExtDef->cld)[1]->_tokenName, "SEMI")) {
        // 函数外变量声明、结构体定义不进行处理
    }
    // Specifire FunDec Compst
    else if (ExtDef->ncld == 3 &&
             !strcmp((ExtDef->cld)[1]->_tokenName, "FunDec")) {
        InterCode code1 = translate_FunDec((ExtDef->cld)[1]);
        InterCode code2 = translate_CompSt((ExtDef->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}

// 变量、函数声明的翻译模式
InterCode translate_FunDec(pnode FunDec)
{
    // ID LP VarList RP
    if (FunDec->ncld == 4) {
        Operand function = new_Variable((FunDec->cld)[0]->content);
        InterCode code1 = new_Code();
        code1->kind = _FUNCTION;
        code1->operands.var = function;
        InterCode code2 = translate_VarList((FunDec->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    // ID LP RP
    else if (FunDec->ncld == 3) {
        Operand function = new_Variable((FunDec->cld)[0]->content);
        InterCode code1 = new_Code();
        code1->kind = _FUNCTION;
        code1->operands.var = function;
        return code1;
    }
    return new_Code();
}
InterCode translate_VarList(pnode VarList)
{
    // ParamDec
    if (VarList->ncld == 1) {
        return translate_ParamDec((VarList->cld)[0]);
    }
    // ParamDec COMMA VarList
    else if (VarList->ncld == 3) {
        InterCode code1 = translate_ParamDec((VarList->cld)[0]);
        InterCode code2 = translate_VarList((VarList->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
InterCode translate_ParamDec(pnode ParamDec)
{
    // Specifire VarDec
    if (ParamDec->ncld == 2) {
        // VarDec:ID
        // 忽略数组的情况
        pnode ID = ((ParamDec->cld)[1]->cld)[0];
        InterCode code1 = new_Code();
        code1->kind = _PARAM;
        code1->operands.var = new_Variable(ID->content);
        return code1;
    }
    return new_Code();
}

// 作用域的翻译模式
InterCode translate_CompSt(pnode ComSt)
{
    // printf("%d\n",ComSt->ncld);
    // LC DefList StmtList RC
    if (ComSt->ncld == 4) {
        InterCode code1 = translate_DefList((ComSt->cld)[1]);
        InterCode code2 = translate_StmtList((ComSt->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
// 语句列表的翻译模式
InterCode translate_StmtList(pnode StmtList)
{
    // Stmt StmtList
    if (StmtList->ncld == 2) {
        InterCode code1 = translate_Stmt((StmtList->cld)[0]);
        InterCode code2 = translate_StmtList((StmtList->cld)[1]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
// 语句的翻译模式
InterCode translate_Stmt(pnode Stmt)
{
    // Exp SEMI
    if (Stmt->ncld == 2 && !strcmp((Stmt->cld)[1]->_tokenName, "SEMI")) {
        return translate_Exp((Stmt->cld)[0], NULL);
    }
    // Compst
    else if (Stmt->ncld == 1 && !strcmp((Stmt->cld)[0]->_tokenName, "Compst")) {
        // Preorder((Stmt->cld)[0],0);
        return translate_CompSt((Stmt->cld)[0]);
    }
    // RETURN Exp SEMI
    else if (Stmt->ncld == 3 && !strcmp((Stmt->cld)[0]->_tokenName, "RETURN")) {
        // 中间代码优化
        Operand existOp = get_Operand((Stmt->cld)[1]);
        if (existOp == NULL) {
            Operand t1 = new_tempvar();
            InterCode code1 = translate_Exp((Stmt->cld)[1], t1);
            InterCode code2 = new_Code();
            code2->kind = _RETURN;
            code2->operands.var = t1;
            return add_Codes(2, code1, code2);
        } else {
            InterCode code1 = new_Code();
            code1->kind = _RETURN;
            code1->operands.var = existOp;
            return code1;
        }
    }
    // IF LP Exp RP Stmt
    else if (Stmt->ncld == 5 && !strcmp((Stmt->cld)[0]->_tokenName, "IF")) {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        InterCode code1 = translate_Cond((Stmt->cld)[2], lable1, lable2);
        InterCode code2 = translate_Stmt((Stmt->cld)[4]);
        return add_Codes(4, code1, new_lable_Code(lable1), code2,
                         new_lable_Code(lable2));
    }
    // IF LP Exp RP Stmt ELSE Stmt
    else if (Stmt->ncld == 7 && !strcmp((Stmt->cld)[0]->_tokenName, "IF")) {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        Operand lable3 = new_lable();
        InterCode code1 = translate_Cond((Stmt->cld)[2], lable1, lable2);
        // print_Codes(code1);
        InterCode code2 = translate_Stmt((Stmt->cld)[4]);
        InterCode code3 = translate_Stmt((Stmt->cld)[6]);
        return add_Codes(7, code1, new_lable_Code(lable1), code2,
                         new_goto_Code(lable3), new_lable_Code(lable2), code3,
                         new_lable_Code(lable3));
    }
    // WHILE LP Exp RP Stmt
    else if (Stmt->ncld == 5 && !strcmp((Stmt->cld)[0]->_tokenName, "WHILE")) {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        Operand lable3 = new_lable();
        InterCode code1 = translate_Cond((Stmt->cld)[2], lable2, lable3);
        InterCode code2 = translate_Stmt((Stmt->cld)[4]);
        return add_Codes(6, new_lable_Code(lable1), code1,
                         new_lable_Code(lable2), code2, new_goto_Code(lable1),
                         new_lable_Code(lable3));
    }
    return new_Code();
}

// 变量声明、初始化的翻译模式
InterCode translate_DefList(pnode DefList)
{
    // Def DefList
    if (DefList->ncld == 2) {
        InterCode code1 = translate_Def((DefList->cld)[0]);
        InterCode code2 = translate_DefList((DefList->cld)[1]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
InterCode translate_Def(pnode Def)
{
    // Specifire DecList SEMI
    if (Def->ncld == 3) {
        return translate_DecList((Def->cld)[1]);
    }
    return new_Code();
}
InterCode translate_DecList(pnode DecList)
{
    // Dec
    if (DecList->ncld == 1) {
        return translate_Dec((DecList->cld)[0]);
    }
    // Dec COMMA DecList
    else if (DecList->ncld == 3) {
        InterCode code1 = translate_Dec((DecList->cld)[0]);
        InterCode code2 = translate_DecList((DecList->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
InterCode translate_Dec(pnode Dec)
{
    // VarDec ASSIGNOP Exp
    if (Dec->ncld == 3) {
        // 没有处理数组
        // VarDec:ID
        pnode ID = ((Dec->cld)[0]->cld)[0];
        Operand variable = new_Variable(ID->content);
        Operand t1 = new_tempvar();
        InterCode code1 = translate_Exp((Dec->cld)[2], t1);
        InterCode code2 = new_assign_Code(variable, t1);
        return add_Codes(2, code1, code2);
    }
    // VarDec
    return new_Code();
}

// 当Exp的翻译模式为INT、ID、MINUS Exp时，可以获取已经申明过的操作数
Operand get_Operand(pnode Exp)
{
    // INT
    if (Exp->ncld == 1 && !strcmp((Exp->cld)[0]->_tokenName, "INT")) {
        return find_Const((int)((Exp->cld)[0]->value));
    }
    // ID
    else if (Exp->ncld == 1 && !strcmp((Exp->cld)[0]->_tokenName, "ID")) {
        Operand variable = new_Variable((Exp->cld)[0]->content);
        return variable;
    }
    // MINUS Exp  (Exp:INT)
    else if (Exp->ncld == 2 && !strcmp((Exp->cld)[0]->_tokenName, "MINUS")) {
        if (!strcmp(((Exp->cld)[1]->cld)[0]->_tokenName, "INT")) {
            int value = -(int)(((Exp->cld)[1]->cld)[0]->value);
            Operand result = find_Const(value);
            if (result == NULL)
                return new_Const(value);
            else
                return result;
        }
    }
    return NULL;
}
// 查看是否已经声明过同一个常数值的临时变量
Operand find_Const(int value)
{
    int i;
    for (i = 0; i < 100; i++) {
        if (tempvar[i] == -1)
            break;
        if (temp_Operands[i]->kind == TEMPVAR &&
            temp_Operands[i]->value == value)
            return temp_Operands[i];
    }
    return NULL;
}

// 基本表达式的翻译模式
InterCode translate_Exp(pnode Exp, Operand place)
{
    int isCond = 0;
    // INT
    if (Exp->ncld == 1 && !strcmp((Exp->cld)[0]->_tokenName, "INT")) {
        Operand value = new_Const((Exp->cld)[0]->value);
        InterCode code = new_assign_Code(place, value);
        return code;
    }
    // ID
    else if (Exp->ncld == 1 && !strcmp((Exp->cld)[0]->_tokenName, "ID")) {
        Operand variable = new_Variable((Exp->cld)[0]->content);
        InterCode code = new_assign_Code(place, variable);
        return code;
    }
    // Exp1 ASSIGNOP Exp2
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "ASSIGNOP")) {
        // Exp1 -> ID
        if ((Exp->cld)[0]->ncld == 1 &&
            !strcmp(((Exp->cld)[0]->cld)[0]->_tokenName, "ID")) {
            Operand variable = new_Variable(((Exp->cld)[0]->cld)[0]->content);
            Operand existOp = get_Operand((Exp->cld)[2]);
            // 中间代码优化
            if (existOp == NULL) {
                Operand t1 = new_tempvar();
                InterCode code1 = translate_Exp((Exp->cld)[2], t1);
                InterCode code2 = new_assign_Code(variable, t1);
                if (place == NULL)
                    return add_Codes(2, code1, code2);
                else {
                    InterCode code3 = new_assign_Code(place, variable);
                    return add_Codes(3, code1, code2, code3);
                }
            } else {
                return new_assign_Code(variable, existOp);
            }
        }
    }
    // Exp PLUS Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "PLUS")) {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL) {
            InterCode code3 = new_Code();
            code3->kind = _ADD;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        } else if (op1 == NULL && op2 != NULL) {
            Operand t1 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code3 = new_Code();
            code3->kind = _ADD;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        } else if (op1 != NULL && op2 == NULL) {
            Operand t2 = new_tempvar();
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _ADD;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        } else {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _ADD;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    // Exp MINUS Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "MINUS")) {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL) {
            InterCode code3 = new_Code();
            code3->kind = _SUB;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        } else if (op1 == NULL && op2 != NULL) {
            Operand t1 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code3 = new_Code();
            code3->kind = _SUB;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        } else if (op1 != NULL && op2 == NULL) {
            Operand t2 = new_tempvar();
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _SUB;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        } else {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _SUB;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    // Exp STAR Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "STAR")) {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL) {
            InterCode code3 = new_Code();
            code3->kind = _MUL;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        } else if (op1 == NULL && op2 != NULL) {
            Operand t1 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code3 = new_Code();
            code3->kind = _MUL;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        } else if (op1 != NULL && op2 == NULL) {
            Operand t2 = new_tempvar();
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _MUL;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        } else {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _MUL;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    // Exp DIV Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "DIV")) {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL) {
            InterCode code3 = new_Code();
            code3->kind = _DIV;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        } else if (op1 == NULL && op2 != NULL) {
            Operand t1 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code3 = new_Code();
            code3->kind = _DIV;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        } else if (op1 != NULL && op2 == NULL) {
            Operand t2 = new_tempvar();
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _DIV;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        } else {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            InterCode code3 = new_Code();
            code3->kind = _DIV;
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    // MINUS Exp
    else if (Exp->ncld == 2 && !strcmp((Exp->cld)[0]->_tokenName, "MINUS")) {
        Operand t1 = new_tempvar();
        InterCode code1 = translate_Exp((Exp->cld)[1], t1);
        InterCode code2 = new_Code();
        code2->kind = _SUB;
        code2->operands.binop.result = place;
        code2->operands.binop.op1 = new_Const(0);
        code2->operands.binop.op2 = t1;
        return add_Codes(2, code1, code2);
    }
    // Exp RELOP Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "RELOP")) {
        isCond = 1;
    }
    // NOT Exp
    else if (Exp->ncld == 2 && !strcmp((Exp->cld)[0]->_tokenName, "NOT")) {
        isCond = 1;
    }
    // Exp AND Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "AND")) {
        isCond = 1;
    }
    // Exp OR Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "OR")) {
        isCond = 1;
    }
    // ID LP RP
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "LP")) {
        Operand function = new_Operand();
        function->kind = FUNC;
        function->operand.name = (Exp->cld)[0]->content;
        if (!strcmp(function->operand.name, "read")) {
            // READ函数处理
            InterCode code = new_Code();
            code->kind = _READ;
            code->operands.var = place;
            return code;
        } else {
            // 其他函数处理
            InterCode code = new_Code();
            code->kind = _CALL;
            code->operands.assign.left = place;
            code->operands.assign.right = function;
            return code;
        }
    }
    // ID LP Args RP
    else if (Exp->ncld == 4 && !strcmp((Exp->cld)[2]->_tokenName, "Args")) {
        int i;
        Operand function = new_Operand();
        function->kind = FUNC;
        function->operand.name = (Exp->cld)[0]->content;
        ArgList arg_list = (ArgList)malloc(sizeof(ArgListStru));
        arg_list->num = 0;
        InterCode code1 = translate_Args((Exp->cld)[2], arg_list);
        InterCode code2, code3;
        if (!strcmp(function->operand.name, "write")) {
            code2 = new_Code();
            code2->kind = _WRITE;
            code2->operands.var = (arg_list->list)[0];
            return add_Codes(2, code1, code2);
        } else {
            for (i = 0; i < arg_list->num; i++) {
                code2 = new_Code();
                code2->kind = _ARG;
                code2->operands.var = (arg_list->list)[i];
                code1 = add_Codes(2, code1, code2);
            }
            code3 = new_Code();
            code3->kind = _CALL;
            code3->operands.assign.left = place;
            code3->operands.assign.right = function;
            return add_Codes(2, code1, code3);
        }
    } else {
        printf("不能处理该类型的语句\n");
    }
    if (isCond) {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        InterCode code0 = new_assign_Code(place, new_Const(0));
        InterCode code1 = translate_Cond(Exp, lable1, lable2);
        InterCode code2 = add_Codes(2, new_lable_Code(lable1),
                                    new_assign_Code(place, new_Const(1)));
        return add_Codes(4, code0, code1, code2, new_lable_Code(lable2));
    }
    return new_Code();
}
// 条件表达式的翻译模式
InterCode translate_Cond(pnode Exp, Operand lable_true, Operand lable_false)
{
    // Exp RELOP Exp
    if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "RELOP")) {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        InterCode code3 = new_Code();
        code3->kind = _IFGOTO;
        code3->operands.jump.lable = lable_true;
        code3->operands.jump.relop = (Exp->cld)[1]->content;
        // 中间代码优化
        if (op1 == NULL && op2 == NULL) {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            code3->operands.jump.op1 = t1;
            code3->operands.jump.op2 = t2;
            return add_Codes(4, code1, code2, code3,
                             new_goto_Code(lable_false));
        } else if (op1 == NULL && op2 != NULL) {
            Operand t1 = new_tempvar();
            InterCode code1 = translate_Exp((Exp->cld)[0], t1);
            code3->operands.jump.op1 = t1;
            code3->operands.jump.op2 = op2;
            return add_Codes(3, code1, code3, new_goto_Code(lable_false));
        } else if (op1 != NULL && op2 == NULL) {
            Operand t2 = new_tempvar();
            InterCode code2 = translate_Exp((Exp->cld)[2], t2);
            code3->operands.jump.op1 = op1;
            code3->operands.jump.op2 = t2;
            return add_Codes(3, code2, code3, new_goto_Code(lable_false));
        } else if (op1 != NULL && op2 != NULL) {
            code3->operands.jump.op1 = op1;
            code3->operands.jump.op2 = op2;
            return add_Codes(2, code3, new_goto_Code(lable_false));
        }
    }
    // NOT Exp
    else if (Exp->ncld == 2 && !strcmp((Exp->cld)[0]->_tokenName, "NOT")) {
        return translate_Cond((Exp->cld)[1], lable_false, lable_true);
    }
    // Exp AND Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "AND")) {
        Operand lable1 = new_lable();
        InterCode code1 = translate_Cond((Exp->cld)[0], lable1, lable_false);
        InterCode code2 =
            translate_Cond((Exp->cld)[2], lable_true, lable_false);
        return add_Codes(3, code1, new_lable_Code(lable1), code2);
    }
    // Exp OR Exp
    else if (Exp->ncld == 3 && !strcmp((Exp->cld)[1]->_tokenName, "OR")) {
        Operand lable1 = new_lable();
        InterCode code1 = translate_Cond((Exp->cld)[0], lable_true, lable1);
        InterCode code2 =
            translate_Cond((Exp->cld)[2], lable_true, lable_false);
        return add_Codes(3, code1, new_lable_Code(lable1), code2);
    }
    // orther cases
    else {
        Operand t1 = new_tempvar();
        InterCode code1 = translate_Exp(Exp, t1);
        InterCode code2 = new_Code();
        char *relop = "!=";
        code2->kind = _IFGOTO;
        code2->operands.jump.lable = lable_true;
        code2->operands.jump.relop = relop;
        code2->operands.jump.op1 = t1;
        code2->operands.jump.op2 = new_Const(0);
        return add_Codes(3, code1, code2, new_goto_Code(lable_false));
    }
}
// 函数参数的翻译模式
InterCode translate_Args(pnode Args, ArgList arg_list)
{
    // Exp
    if (Args->ncld == 1) {
        Operand existOp = get_Operand((Args->cld)[0]);
        if (existOp != NULL) {
            if (existOp->kind == CONSTANT) {
                Operand t1 = new_tempvar();
                InterCode code1 = new_assign_Code(t1, existOp);
                arg_list->list[arg_list->num] = t1;
                arg_list->num++;
                return code1;
            }
            arg_list->list[arg_list->num] = existOp;
            arg_list->num++;
            return new_Code();
        }
        Operand t1 = new_tempvar();
        InterCode code1 = translate_Exp((Args->cld)[0], t1);
        arg_list->list[arg_list->num] = t1;
        arg_list->num++;
        return code1;
    }
    // Exp COMMA Args
    else if (Args->ncld == 3) {
        Operand t1 = new_tempvar();
        InterCode code1 = translate_Exp((Args->cld)[0], t1);
        arg_list->list[arg_list->num] = t1;
        arg_list->num++;
        InterCode code2 = translate_Args((Args->cld)[2], arg_list);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}

/**********************目标代码**************************/
char *Int2String(int num, char *str)  // 10进制
{
    int i = 0;    //指示填充str
    if (num < 0)  //如果num为负数，将num变正
    {
        num = -num;
        str[i++] = '-';
    }
    //转换
    do {
        str[i++] =
            num % 10 +
            48;  //取num最低位
                 //字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0'
        num /= 10;  //去掉最低位
    } while (num);  // num不为0继续循环

    str[i] = '\0';

    //确定开始调整的位置
    int j = 0;
    if (str[0] == '-')  //如果有负号，负号不用调整
    {
        j = 1;  //从第二位开始调整
        ++i;    //由于有负号，所以交换的对称轴也要后移1位
    }
    //对称交换
    for (; j < i / 2; j++) {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b;
        str[j] = str[j] + str[i - 1 - j];
        str[i - 1 - j] = str[j] - str[i - 1 - j];
        str[j] = str[j] - str[i - 1 - j];
    }

    return str;  //返回转换后的值
}

// 分配寄存器
char *allocate_reg(Operand op)
{
    int i;
    char *regnumber = (char *)malloc(sizeof(char) * 10);
    char *regname = (char *)malloc(sizeof(char) * 10);
    strcat(regname, "$t");
    // 常数0 寄存器
    if (op->kind == CONSTANT && op->operand.value == 0)
        return "$zero";
    else if (op->kind == TEMPVAR && op->value == 0)
        return "$zero";
    // 寻找存储该操作数的寄存器
    int find = 0;
    for (i = 0; i < reg_num; i++) {
        if (regs[i] == NULL || regs[i]->kind != op->kind)
            continue;
        if (regs[i]->kind == CONSTANT &&
            regs[i]->operand.value == op->operand.value) {
            find = 1;
            break;
        } else if (regs[i]->kind == TEMPVAR &&
                   regs[i]->operand.tempvar == op->operand.tempvar) {
            find = 1;
            break;
        } else if (regs[i]->kind == VARIABLE &&
                   !strcmp(regs[i]->operand.name, op->operand.name)) {
            find = 1;
            break;
        }
    }
    if (find) {
        Int2String(i, regnumber);
        strcat(regname, regnumber);
        return regname;
    } else {
        Int2String(reg_num, regnumber);
        strcat(regname, regnumber);
        regs[reg_num] = op;
        reg_num++;
        return regname;
    }
}
// 根据中间代码生成mips代码
void generate_MIPS_Codes(InterCode codes, FILE *fp)
{
    fprintf(fp, "目标代码打印：\n");
    // 声明部分
    fprintf(fp, ".data\n_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(fp, "_ret: .asciiz \"\\n\"\n.globl main\n.text\n");
    // read函数
    fprintf(fp, "read:\n    li $v0, 4\n    la $a0, _prompt\n    syscall\n");
    fprintf(fp, "    li $v0, 5\n    syscall\n    jr $ra\n\n");
    // write函数
    fprintf(fp,
            "write:\n    li $v0, 1\n    syscall\n    li $v0, 4\n    la $a0, "
            "_ret\n");
    fprintf(fp, "    syscall\n    move $v0, $0\n    jr $ra\n\n");
    InterCode temp = new_Code();
    temp = codes;
    while (temp != NULL) {
        generate_MIPS_Code(temp, fp);
        temp = temp->next;
    }
    fprintf(fp, "打印完毕\n");
}
// 翻译单条中间代码
void generate_MIPS_Code(InterCode code, FILE *fp)
{
    if (code == NULL) {
        fprintf(fp, "Error, MIPS is NULL\n");
        return;
    }
    switch (code->kind) {
        case _NULL:
            break;
        case _LABLE: {
            print_Operand(fp, code->operands.var);
            fprintf(fp, ":\n");
            break;
        }
        case _FUNCTION: {
            print_Operand(fp, code->operands.var);
            fprintf(fp, ":\n");
            break;
        }
        case _ASSIGN: {
            Operand left = code->operands.assign.left;
            Operand right = code->operands.assign.right;
            if (right->kind == CONSTANT) {
                // 如果将0赋给一个临时变量，则不需要输出该mips代码
                if (left->kind == TEMPVAR && right->value == 0)
                    break;
                else
                    fprintf(fp, "    li %s, %d\n", allocate_reg(left),
                            right->operand.value);
            } else {
                fprintf(fp, "    move %s, %s\n", allocate_reg(left),
                        allocate_reg(right));
            }
            break;
        }
        case _ADD: {
            Operand result = code->operands.binop.result;
            Operand op1 = code->operands.binop.op1;
            Operand op2 = code->operands.binop.op2;
            if (op2->kind == CONSTANT) {
                fprintf(fp, "    addi %s, %s, %d\n", allocate_reg(result),
                        allocate_reg(op1), op2->value);
            } else {
                fprintf(fp, "    add %s, %s, %s\n", allocate_reg(result),
                        allocate_reg(op1), allocate_reg(op2));
            }
            break;
        }
        case _SUB: {
            Operand result = code->operands.binop.result;
            Operand op1 = code->operands.binop.op1;
            Operand op2 = code->operands.binop.op2;
            if (op2->kind == CONSTANT) {
                fprintf(fp, "    addi %s, %s, %d\n", allocate_reg(result),
                        allocate_reg(op1), -(op2->value));
            } else {
                fprintf(fp, "    sub %s, %s, %s\n", allocate_reg(result),
                        allocate_reg(op1), allocate_reg(op2));
            }
            break;
        }
        case _MUL: {
            Operand result = code->operands.binop.result;
            Operand op1 = code->operands.binop.op1;
            Operand op2 = code->operands.binop.op2;
            fprintf(fp, "    mul %s, %s, %s\n", allocate_reg(result),
                    allocate_reg(op1), allocate_reg(op2));
            break;
        }
        case _DIV: {
            Operand result = code->operands.binop.result;
            Operand op1 = code->operands.binop.op1;
            Operand op2 = code->operands.binop.op2;
            fprintf(fp, "    div %s, %s\n", allocate_reg(op1),
                    allocate_reg(op2));
            fprintf(fp, "    mflo %s\n", allocate_reg(result));
            break;
        }
        case _GOTO: {
            Operand lable = code->operands.jump.lable;
            fprintf(fp, "    j ");
            print_Operand(fp, lable);
            fprintf(fp, "\n");
            break;
        }
        case _CALL: {
            break;
        }
        case _READ: {
            Operand op = code->operands.var;
            fprintf(fp, "    addi $sp, $sp, -4\n");
            fprintf(fp, "    sw $ra, 0($sp)\n");
            fprintf(fp, "    jal read\n");
            fprintf(fp, "    lw $ra, 0($sp)\n");
            fprintf(fp, "    addi $sp, $sp, 4\n");
            fprintf(fp, "    move %s, $v0\n", allocate_reg(op));
            break;
        }
        case _WRITE: {
            Operand op = code->operands.var;
            fprintf(fp, "    move $a0, %s\n", allocate_reg(op));
            fprintf(fp, "    addi $sp, $sp, -4\n");
            fprintf(fp, "    sw $ra, 0($sp)\n");
            fprintf(fp, "    jal write\n");
            fprintf(fp, "    lw $ra, 0($sp)\n");
            fprintf(fp, "    addi $sp, $sp, 4\n");
            break;
        }
        case _RETURN: {
            Operand res = code->operands.var;
            fprintf(fp, "    move $v0, %s\n", allocate_reg(res));
            fprintf(fp, "    jr $ra\n");
            break;
        }
        case _IFGOTO: {
            char *op = code->operands.jump.relop;
            Operand lable = code->operands.jump.lable;
            Operand op1 = code->operands.jump.op1;
            Operand op2 = code->operands.jump.op2;
            if (!strcmp(op, "==")) {
                fprintf(fp, "    beq %s, %s, ", allocate_reg(op1),
                        allocate_reg(op2));
                print_Operand(fp, lable);
                fprintf(fp, "\n");
            } else if (!strcmp(op, "!=")) {
                fprintf(fp, "    bne %s, %s, ", allocate_reg(op1),
                        allocate_reg(op2));
                print_Operand(fp, lable);
                fprintf(fp, "\n");
            } else if (!strcmp(op, ">")) {
                fprintf(fp, "    bgt %s, %s, ", allocate_reg(op1),
                        allocate_reg(op2));
                print_Operand(fp, lable);
                fprintf(fp, "\n");
            } else if (!strcmp(op, "<")) {
                fprintf(fp, "    blt %s, %s, ", allocate_reg(op1),
                        allocate_reg(op2));
                print_Operand(fp, lable);
                fprintf(fp, "\n");
            } else if (!strcmp(op, ">=")) {
                fprintf(fp, "    bge %s, %s, ", allocate_reg(op1),
                        allocate_reg(op2));
                print_Operand(fp, lable);
                fprintf(fp, "\n");
            } else if (!strcmp(op, "<=")) {
                fprintf(fp, "    ble %s, %s, ", allocate_reg(op1),
                        allocate_reg(op2));
                print_Operand(fp, lable);
                fprintf(fp, "\n");
            }
            break;
        }
        default:
            break;
    }
}
