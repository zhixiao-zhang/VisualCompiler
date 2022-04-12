#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

extern int _yyline;
extern char *yytext;
void yyerror(char *msg);

typedef struct parserNode {
  int line;
  char *_tokenName;
  struct parserNode *fchild, *next;
  union  {
    char *id_type;
    int interval;
  };
} *Ast, *pnode;

Ast newAst(char *name, int number, ...);

void PreOrder(Ast ast, int level);

int _nodeNum;

pnode nodeList[10000];
int nodeChild[10000];

void Childset(pnode node);

int hasFault;
