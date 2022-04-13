#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

//行数;
extern int yylineno;
//文字;
extern char *yytext;
//错误处理函数
void yyerror(char *msg);

//语法树
typedef struct parserNode {
  int line;
  char *_tokenName;
  struct parserNode *fchild, *next;
  union  {
    char *id_type;
    int interval;
  };
} *Ast, *pnode;

//创建新的节点
Ast newAst(char *name, int number, ...);

//先序遍历语法树
void PreOrder(Ast ast, int level);

//节点个数
int _nodeNum;

//存放节点
pnode nodeList[10000];
int nodeChild[10000];

//设置状态
void Childset(pnode node);

//是否有错误
int hasFault;
