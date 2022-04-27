#include "../include/compiler.h"

/*test*/
int i;

/*
 *用于在语法树中创建一个节点;
 */
Ast newAst(char *name, int number, ...) {
  //创建父亲节点;
  pnode father = (pnode)malloc(sizeof(struct parserNode));
  pnode tmp = (pnode)malloc(sizeof(struct parserNode));
  if (!father) {
    yyerror("Error on creating node!!!");
    exit(-1);
  }
  father->_tokenName = name;

  //参数列表;
  va_list list;
  //初始化参数列表;
  va_start(list, number);

  //当前节点为非终结符
  if (number > 0) {
    //第一个孩子节点;
    tmp = va_arg(list, pnode);
    Childset(tmp);
    //父亲节点的行号为第一个孩子节点的行号;
    father->line = tmp->line;

    if (number >= 2) {
      for (i = 0; i < number - 1; i++) {
        tmp->next = va_arg(list, pnode);
        tmp = tmp->next;
        Childset(tmp);
      }
    }
  } else {
    father->line = va_arg(list, int);
    if ((!strcmp(name, "ID")) || (!strcmp(name, "TYPE"))) {
      char *tmpstr;
      tmpstr = (char *)malloc(sizeof(char) * 40);
      strcpy(tmpstr, yytext);
      father->id_type = tmpstr;
    } else if (!strcpy(name, "INT")) {
      father->interval = atoi(yytext);
    }
  }
  return father;
}

void PreOrder(Ast ast, int level) {
  if (ast != NULL) {
    for (i = 0; i < level; i++) {
      printf(" ");
    }
    if (ast->line != -1) { //-1代表空节点
      printf("%s", ast->_tokenName);
      if ((!strcmp(ast->_tokenName, "ID")) || (!strcmp(ast->_tokenName, "TYPE"))) {
        printf(": %s", ast->id_type);
      } else if (!strcmp(ast->_tokenName, "INT")) {
        printf(": %d", ast->interval);
      } else {
        printf("(%d)", ast->line);
      }
    }
    printf("\n");
    PreOrder(ast->fchild, level + 1);
    PreOrder(ast->next, level);
  }
}

void yyerror(char *msg) {
  hasFault = 1;
  fprintf(stderr, "Error type b at line %d, %s, before %s\n", yylineno, msg, yytext);
}

void Childset(pnode node) {
  int i;
  for (i = 0; i < _nodeNum; i++) {
    if (nodeList[i] == node) {
      nodeChild[i] = 1;
    }
  }
}

int main(int argc, char *argv[]) {
  int j;
  puts("The analysis starts!");
  if (argc < 2) {
    return 1;
    //未提供文件;
  }
  for (i = 1; i < argc; i++) { //可能存在的多文件;
    _nodeNum = 0;
    memset(nodeList, 0, sizeof(pnode) * 10000);
    memset(nodeChild, 0, sizeof(int) * 10000);
    hasFault = 0;

    FILE *f  = fopen(argv[1], "r");
    if (!f) {
      perror(argv[1]);
      return 1;
    }
    yyrestart(f);
    yyparse();
    fclose(f);

    if (hasFault) {
      continue;
    }
    
    for (j = 0; j < _nodeNum; j++) {
      if (nodeChild[j] != 1) {
        PreOrder(nodeList[j], 0);
      }
    }
  }
}
