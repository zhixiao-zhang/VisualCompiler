%{
#include <stdio.h>
#include <stdlib.h>
#include "../include/compiler.h"
#include "compiler.tab.h"

//外部变量声明，yytext和yyin分别为字符指针和文件指针
//这两个变量在lex提供的库值被定义，因此想要使用必须使用extern

%}

 /*flex属性,用以记录符号所在行号*/
%option yylineno

 /*定义正则表达式*/
 /*十进制*/
DIGIT [1-9]
DIGITS [0-9]
INT 0|({DIGIT}[DIGITS]*)
 /*标识符*/
LETTER_ [A-Za-z_]
ID {LETTER_}({LETTER_}|{DIGITS})*

 /*关键字*/
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
TYPE int

 /*符号*/
SEMI ;
COMMA ,
ASSIGNOP =
PLUS \+
MINUS \-
STAR \*
DIV \/
AND &&
GETMEMBER \.
NOT \!
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
RELOP >|<|>=|<=|==|!=

 /*注释*/
COMMENT ("//".*)|("/*"([*]*(([^*/])+([/])*)*)*"*/")

 /*空白符*/
DELIM [ \t\f\v\r]
SPACE {DELIM}+

 /*换行*/
EOL \n

 /*未定义字符*/
AERROR .

 /*第三部分 操作*/
%%
 /*跳过空白和注释*/
{SPACE} {}
{COMMENT} {}
{EOL} {}
 /*关键字*/
{TYPE} {yylval.type_pnode = newAst("TYPE", 0, yylineno); return TYPE;}
{STRUCT} {yylval.type_pnode = newAst("STRUCT", 0, yylineno); return STRUCT;}
{RETURN} {yylval.type_pnode = newAst("RETURN", 0, yylineno); return RETURN;}
{IF} {yylval.type_pnode = newAst("IF", 0, yylineno); return IF;}
{ELSE} {yylval.type_pnode = newAst("ELSE", 0, yylineno); return ELSE;}
{WHILE} {yylval.type_pnode = newAst("WHILE", 0, yylineno); return WHILE;}
 /*数字类型表示*/
{INT} {yylval.type_pnode = newAst("INT", 0, yylineno); return INT;}
 /*符号*/
{SEMI} {yylval.type_pnode = newAst("SEMI", 0, yylineno); return SEMI;}
{COMMA} {yylval.type_pnode = newAst("COMMA", 0, yylineno); return COMMA;}
{ASSIGNOP} {yylval.type_pnode = newAst("ASSIGNOP", 0, yylineno); return ASSIGNOP;}
{PLUS} {yylval.type_pnode = newAst("PLUS", 0, yylineno); return PLUS;}
{MINUS} {yylval.type_pnode = newAst("MINUS", 0, yylineno); return MINUS;}
{STAR} {yylval.type_pnode = newAst("STAR", 0, yylineno); return STAR;}
{DIV} {yylval.type_pnode = newAst("DIV", 0, yylineno); return DIV;}
{AND} {yylval.type_pnode = newAst("AND", 0, yylineno); return AND;}
{GETMEMBER} {yylval.type_pnode = newAst("GETMEMBER", 0, yylineno); return GETMEMBER;}
{NOT} {yylval.type_pnode = newAst("NOT", 0, yylineno); return NOT;}
{LP} {yylval.type_pnode = newAst("LP", 0, yylineno); return LP;}
{RP} {yylval.type_pnode = newAst("RP", 0, yylineno); return RP;}
{LB} {yylval.type_pnode = newAst("LB", 0, yylineno); return LB;}
{RB} {yylval.type_pnode = newAst("RB", 0, yylineno); return RB;}
{LC} {yylval.type_pnode = newAst("LC", 0, yylineno); return LC;}
{RC} {yylval.type_pnode = newAst("RC", 0, yylineno); return RC;}
{RELOP} {yylval.type_pnode = newAst("RELOP", 0, yylineno); return RELOP;}

 /*标识符*/
{ID} {yylval.type_pnode = newAst("ID", 0, yylineno); return ID;}
 /*错误表示*/
{AERROR} {
  hasFault = 1;
  printf("Error type A at line %d: Mystirious charachter '%s'\n", yylineno, yytext);
}
%%

/*第四部分 函数 定义yywrap函数，此函数在lex的库中只进行了声明而没有进行定义*/
int yywrap() {
  return 1;
}
/*由于flex将要由bison调用,故不用添加main函数*/
