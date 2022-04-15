INT
TYPE STRUCT RETURN IF ELSE WHILE ID COMMENT SPACE SEMI COMMA ASSIGNOP PLUS
MINUS STAR DIV AND OR GETMEMEBER NOT LP RP LB RB LC RC AERROR RELOP EOL
Program ExtDefList ExtDef ExtDecList Specifire StructSpecifire
OptTag Tag VarDec FuncDeclare VarList ParamDeclare Compst StmtList Stmt DefList Defition DecList Dec Exp Args

1. Program->ExtDefList

2. ExtDefList->ExtDef ExtDefList

​						|(∊)

3. ExtDef->Specifire ExtDecList SEMI

​				|Specifire SEMI

​				|Specifire FuncDec Compst

4. ExtDecList->VarDec

​						|VarDec COMMA ExtDecList

/*Speifire\*/

5. Specifire->TYPE

​					|StructSpecifire

6. StructSpecifire->STRUCT OptTag LC DefList RC

​								|STRUCT Tag

7. OptTag->ID

​				|(∊)

8. Tag->ID

/*Declarators\*/

9. VarDec->ID

​				   |VarDec LB INT RB

10. FuncDec->ID LP VarList RP

​			 		|ID LP RP

11. VarList->ParamDec COMMA VarList

​		  		|ParamDec

12. ParamDec->Specifire VarDec

/*Statement\*/

13. Compst->LC DefList StmtList RC

14. StmtList->Stmt StmtList

​					|(∊)

15. Stmt->Exp SEMI

​			 |Compst

​			 |RETURN Exp SEMI

​			 |IF LP Exp RP Stmt %prec LOWER_THAN_ELSE

​			 |IF LP Exp RP Stmt ELSE Stmt

​			 |WHILE LP Exp RP Stmt

16. DefList->Def DefList

​					|(∊)

17. Def->Specifire DecList SEMI

18. DecList->Dec

    ​			 |Dec COMMA DecList

19. Dec->VarDec

    ​		|VarDec ASSIGNOP Exp

/*Expression\*/

20. Exp->Exp ASSIGNOP Exp

​	  	 |Exp AND Exp

​	  	 |Exp OR Exp

​	  	 |EXP RELOP Exp

​	  	 |Exp PLUS Exp

​	  	 |Exp MINUS Exp

​	  	 |Exp STAR Exp

​	  	 |Exp DIV Exp

​	  	 |LP Exp RP

​	  	 |MINUS Exp

​	  	 |NOT Exp

​	  	 |ID LP Args RP

​	  	 |ID LP RP

​	  	 |Exp LB Exp RB

​	  	 |Exp LB Exp RB

​	  	 |Exp GETMEMBER ID

​	  	 |ID

​	  	 |INT

​	  	 |FLOAT

21. Args->EXP COMMA Args

​		 	|Exp

BNF范式解释：

1. Program 起始符号
2. 全局定义列表 全局定义 全局定义列表   /   (∊)
3. 全局定义 标识符 全局声明列表（多个变量） 分号      /      结构声明 分号    /     返回类型 函数名 函数体
4. 全局声明列表（多个变量）变量声明   /  变量声明 逗号 变量声明（多个变量）
5. 标识符 类型   /  结构标识符
6. 结构标识符 STRUCT 名字 大括号 内容 大括号  /  STRUCT 名字
7. 名字 ID
8. 名字 ID
9. 变量声明 名字   /   名字 左中括号 数字 右中括号（数组）
10. 函数声明 名字 左小括号 参数列表 右小括号   /   名字 左小括号 右小括号
11. 参数列表 形参声明 逗号 参数列表（右递归）   /   形参声明
12. 形参声明 标识符 变量声明
13. 函数体 左大括号 定义列表 语句列表 右大括号
14. 语句列表 语句（可以是一条语句也可以是一个语句块）   /   (∊)
15. 语句 表达式 分号   /   函数体   /   RETURN 表达式 分号   /   没有ELSE的if语句   /   有ELSE的if语句   /   WHILE 左小括号 语句 右小括号 语句
16. 定义列表 定义 定义列表   /   (∊)
17. 定义 标识符 声明列表 分号
18. 声明列表 声明   /   声明 逗号 声明列表（多个声明）
19. 声明 变量声明 等号 表达式
20. 语句 十分明显 无需翻译
21. 实参列表 表达式 逗号 实参列表   /   表达式