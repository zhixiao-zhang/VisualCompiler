%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//外部变量声明，yytext和yyin分别为字符指针和文件指针
//这两个变量在lex提供的库值被定义，因此想要使用必须使用extern
extern char *yytext;
extern FILE *yyin;

%}

digit [0-9]
digits [+-]?{digit}+
delim [ \t\n]
ws {delim}+
letter_ [A-Za-z_]
id {letter_}({letter_}|{digit})*

%%
"int" {printf("INT\n");}
"void" {printf("VOID\n");}
"main" {printf("ID\n");}
"return" {printf("RETURN\n");}
"for" {printf("FOR\n");}
"while" {printf("WHILE\n");}
"if" {printf("IF\n");}
"else" {printf("ELSE\n");}
"struct" {printf("STRUCT\n");}
";" {printf("SEMI\n");}
"," {printf("COMMA\n");}
"=" {printf("ASSIGNOP\n");}
(>=)|(<=)|(==)|(!=)|<|> {printf("RELOP\n");}
"+" {printf("PLUS\n");}
"-" {printf("MINUS\n");}
"*" {printf("STAR\n");}
"/" {printf("DIV\n");}
"%" {printf("MOD\n");}
"^" {printf("POWER\n");}
"&&" {printf("AND\n");}
"&" {printf("SINGALAND\n");}
"||" {printf("OR\n");}
"!" {printf("NOT\n");}
"(" {printf("LP\n");}
")" {printf("RP\n");}
"[" {printf("LB\n");}
"]" {printf("RB\n");}
"{" {printf("LC\n");}
"}" {printf("RC\n");}
"." {printf("GETMEMBER\n");}
"++" {printf("SELFPLUS\n");}
"--" {printf("SELFMINUS\n");}

[\t\v\f\r\n] { }
{ws} {}
{id} {printf("ID\n");}
{digits} {printf("NUMBER\n");}
%%

//定义yywrap函数，此函数在lex的库中只进行了声明而没有进行定义
int yywrap() {
  return 1;
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    printf("ERROR, argument too much!\n");
  } else if (argc == 2){
    yyin = fopen(argv[1], "rb+");
  }
  yylex();

  return 0;
}
