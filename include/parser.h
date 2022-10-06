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
  int line; /*记录该节点在语法树中的行号*/
  char *_tokenName; /*flex返回的token的名称*/
  int tag;   //1.变量 2.函数 3.常数 4.数组 5.结构体
  struct parserNode *cld[10];    //使用数组表示孩子结点
  int ncld;
  //语义值
  char *content;
  //数据类型
  char *type;
  //变量的值
  float value;
} *Ast, *pnode;

//创建新的节点
Ast newAst(char *name, int number, ...);

//先序遍历语法树(深度优先搜索)
void PreOrder(Ast ast, int level, FILE *fp);
void Preorder1(Ast ast, int level);

//节点个数
int  _nodeNum;

//存放节点
pnode nodeList[10000];
int nodeIsChild[10000];

//设置状态
void Childset(pnode node);

//是否有错误
int hasFault;
