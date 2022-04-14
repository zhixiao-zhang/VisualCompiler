%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//外部变量声明，yytext和yyin分别为字符指针和文件指针
//这两个变量在lex提供的库值被定义，因此想要使用必须使用extern
extern char *yytext;
extern FILE *yyin;

%}

 /*flex属性,用以记录符号所在行号*/
%option yylineno

 /*定义正则表达式*/
 /*十进制*/
DIGIT [0-9]
DIGITS ({digit}+)(\.{digit}+)?(E[+-]?{digit}+)?

 /*标识符*/
LETTER_ [A-Za-z_]
ID {LETTER_}({LETTER_}|{DIGIT})*

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
{TYPE} {return TYPE;}
{STRUCT} {return STRUCT;}
{RETURN} {return RETURN;}
{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}
 /*数字类型表示*/
{INT} {return INT;}
 /*符号*/
{SEMI} {return SEMI;}
{COMMA} {return COMMA;}
{ASSIGNOP} {return ASSIGNOP;}
{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{STAR} {return STAR;}
{DIV} {return DIV;}
{AND} {return AND;}
{GETMEMBER} {return GETMEMBER;}
{NOT} {return NOT;}
{LP} {return LP;}
{RP} {return RP;}
{LB} {return LB;}
{RB} {return Rb;}
{LC} {return LC;}
{RC} {return RC;}
{RELOP} {return RELOP;}

 /*标识符*/
{ID} {return ID;}
 /*错误表示*/
{AERROR} {
  hasFault = 1;
  printf("Error type A at line %d: Mystirious charachter '%s'\n", _yylineno, yytext);
}
%%

/*第四部分 函数 定义yywrap函数，此函数在lex的库中只进行了声明而没有进行定义*/
int yywrap() {
  return 1;
}
/*由于flex将要由bison调用,故不用添加main函数*/
