%{
#include <unistd.h>
#include <stdio.h>
#include "../include/semantic.h"
extern int yylex();
%}
%union{
pnode type_pnode;
double d;
}
/*声明记号的值*/
%token <type_pnode> INT FLOAT
%token <type_pnode> TYPE STRUCT RETURN IF ELSE WHILE ID COMMENT SPACE SEMI COMMA ASSIGNOP PLUS
%token <type_pnode> MINUS STAR DIV AND OR GETMEMBER NOT LP RP LB RB LC RC AERROR RELOP EOL
%type <type_pnode> Program ExtDefList ExtDef ExtDecList Specifire StructSpecifire
%type <type_pnode> OptTag Tag VarDec FunDec VarList ParamDec Compst StmtList Stmt DefList Def DecList Dec Exp Args

/*规定优先级*/
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB GETMEMBER
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
/*产生式*/
%%
Program:ExtDefList {
       $$=newAst("Program",1,$1);
       }
       ;
ExtDefList:ExtDef ExtDefList {
          $$=newAst("ExtDefList",2,$1,$2);
          } 
          | {
          $$=newAst("ExtDefList",0,-1);
          }
          ;
ExtDef:Specifire ExtDecList SEMI {
      $$=newAst("ExtDef",3,$1,$2,$3);
      }
    |Specifire SEMI {
    $$=newAst("ExtDef",2,$1,$2);
    }
    |Specifire FunDec Compst {
    $$=newAst("ExtDef",3,$1,$2,$3);
    newfunc(1, $1);
    }
    ;
ExtDecList:VarDec {
          $$ = newAst("ExtDecList", 1, $1);
          if (ifvardef($1)) {
            printf("Error type 7 at Line %d: Redefined variable '%s'\n", yylineno, $1->content);
            exit(-1);
          }
          else {
            newvar(1, $1);
          }
    }
    |VarDec COMMA ExtDecList {
    $$ = newAst("ExtDecList", 3, $1, $2, $3);
    }
          ;

/*说明符*/
Specifire:TYPE {
         $$ = newAst("Specifire", 1, $1);
         }
    | StructSpecifire {
    $$ = newAst("Specifire", 1, $1);
    }
    ;
StructSpecifire:STRUCT OptTag LC DefList RC {
               inStruc = 0;
               $$ = newAst("StructSpecifire", 5, $1, $2, $3, $4, $5);
               if (findstruc($2)) {
                  printf("Error type 11 at line %d:Dulpicated name '%s'\n", yylineno, $2->content);
                  exit(-1);
                } else {
                  newstruc(1, $2);
                }
               }
               |STRUCT Tag {
               $$ = newAst("StructSpecifire", 2, $1, $2);
               }
               ;
OptTag:ID {
      $$ = newAst("OptTag", 1, $1);
      }
      | {
      $$ = newAst("OptTag", 0, -1);
      }
      ;
Tag:ID {
   $$ = newAst("Tag", 1, $1);
   }
   ;

/*声明符*/
VarDec:ID {
      $$ = newAst("VarDec", 1, $1);
      $$->tag = 1;
      $$->content = $1->content;
      }
      |VarDec LB INT RB {
      $$ = newAst("VarDec", 4, $1, $2, $3, $4);
      $$->content = $1->content;
      $$->tag = 4;
      }
      ;
FunDec:ID LP VarList RP {
      $$ = newAst("FunDec", 4, $1, $2, $3, $4);
      $$->content = $1->content;
      if (findfunc($1)) {
          printf("Error type 8 line %d:Redefined Function '%s'\n ", yylineno, $1->content);
          exit(-1);
        } else {
          newfunc(2, $1, $3);
        }
      }
      |ID LP RP {
           $$ = newAst("FunDec", 3, $1, $2, $3);
           $$->content = $1->content;
           if (findfunc($1)) {
                    printf("Error type 8 line %d:Redefined Function '%s'\n ", yylineno, $1->content);
                    exit(-1);
              } else {
                newfunc(2, $1, $3);
              }
           }
           ;
VarList:ParamDec COMMA VarList {
       $$ = newAst("VarList", 3, $1, $2, $3);
       }
       |ParamDec {
       $$ = newAst("VarList", 1, $1);
       }
       ;
ParamDec:Specifire VarDec {
        $$ = newAst("ParamDec", 2, $1, $2);
        if (ifvardef($2) || ifarraydef($2)) {
            printf("Error type 7 at line %d:Redefined Variable '%s'\n", yylineno, $2->content);
            exit(-1);
          } else if($2->tag == 4) {
            newarray(2, $1, $2);
          } else {
            newvar(2, $1, $2);
          }
        }
        ;

/*声明*/
Compst:LC DefList StmtList RC {
      $$ = newAst("Compst", 4, $1, $2, $3, $4);
      }
      ;
StmtList:Stmt StmtList {
        $$ = newAst("StmtList", 2, $1, $2);
        }
        | {
        $$ = newAst("StmtList", 0, -1);
        }
        ;
Stmt:Exp SEMI {
    $$ = newAst("Stmt", 2, $1, $2);
    }
    |Compst {
    $$ = newAst("Stmt", 1, $1);
    }
    |RETURN Exp SEMI {
        $$ = newAst("Stmt", 3, $1, $2, $3);
        getrtype($2);
    }
    |IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
        $$ = newAst("Stmt", 5, $1, $2, $3, $4, $5);
    }
    |IF LP Exp RP Stmt ELSE Stmt {
        $$ = newAst("Stmt", 7, $1, $2, $3, $4, $5, $6, $7);
    }
    |WHILE LP Exp RP Stmt {
        $$ = newAst("Stmt", 5, $1, $2, $3, $4, $5);
    }
    ;
DefList:Def DefList {
            $$ = newAst("DefList", 2, $1, $2);
       }
       | {
            $$ = newAst("DefList", 0, -1);
       }
       ;
Def:Specifire DecList SEMI {
   $$ = newAst("Def", 3, $1, $2, $3);
   }
   ;
DecList:Dec {
       $$ = newAst("DecList", 1, $1);
       } 
       |Dec COMMA DecList {
       $$ = newAst("DecList", 3, $1, $2, $3);
       $$->tag = $3->tag;
       }
       ;
Dec:VarDec {
   $$ = newAst("Dec", 1, $1);
   if (ifvardef($1) || ifarraydef($1)) {
      printf("Error type 7 at line %d:Redefined Variable '%s'\n", yylineno, $1->content);
      exit(-1);
     } else if($1->tag == 4) {
        newarray(1, $1);
     } else {
        newvar(1, $1);
     }
   }
   |VarDec ASSIGNOP Exp {
   $$ = newAst("Dec", 3, $1, $2, $3);
   $$->content = $1->content;
   if (ifvardef($1) || ifarraydef($1)) {
      printf("Error type 7 at line %d:Redefined Variable '%s'\n", yylineno, $1->content);
      exit(-1);
     } else if($1->tag == 4) {
        newarray(1, $1);
     } else {
        newvar(1, $1);
     }
   }
   ;
 /*Expression*/
Exp:Exp ASSIGNOP Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   if ($1->type == NULL || $3->type == NULL) {
    } else {
        if (strcmp($1->type, $3->type)) {
            printf("Error type 2 at line %d:Type mismatched for for assignment.\n ", yylineno);
            exit(-1);
        }
        if (!checkleft($1)) {
            printf("Error type 3 at line %d:The left-hand side of an assignment must be a Variable.\n ", yylineno);
            exit(-1);
        }
      }
   }
   |Exp AND Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   }
   |Exp OR Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   }
   |Exp RELOP Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   }
   |Exp PLUS Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   if (strcmp($1->type, $3->type)) {
      printf("Error type 6 at line %d:Type mismatched for for operands.\n ", yylineno);
      exit(-1);
    }
   }
   |Exp MINUS Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   if (strcmp($1->type, $3->type)) {
      printf("Error type 6 at line %d:Type mismatched for for operands.\n ", yylineno);
      exit(-1);
    }
   }
   |Exp STAR Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   if (strcmp($1->type, $3->type)) {
      printf("Error type 6 at line %d:Type mismatched for for operands.\n ", yylineno);
      exit(-1);
    }
   }
   |Exp DIV Exp {
   $$ = newAst("Exp", 3, $1, $2, $3);
   if (strcmp($1->type, $3->type)) {
      printf("Error type 6 at line %d:Type mismatched for for operands.\n ", yylineno);
      exit(-1);
    }
   }
   |LP Exp RP {
   $$ = newAst("Exp", 3, $1, $2, $3);
   }
   |MINUS Exp {
   $$ = newAst("Exp", 2, $1, $2);
   }
   |NOT Exp {
   $$ = newAst("Exp", 2, $1, $2);
   }
   |ID LP Args RP {
   $$ = newAst("Exp", 4, $1, $2, $3, $4);
   if(!findfunc($1) && (ifvardef($1)||ifarraydef($1))) {
            printf("Error type 4 at Line %d:'%s' is not a function.\n ",yylineno,$1->content);
            exit(-1);
    } else if(!findfunc($1)) {
            printf("Error type 5 at Line %d:Undefined function %s\n ",yylineno,$1->content);
            exit(-1);
    } else if(checkrtype($1,$3)){
            printf("Error type 13 at Line %d:Function parameter type error.\n ",yylineno);
            exit(-1);
    } else{}
   }
   |ID LP RP {
   $$ = newAst("Exp", 3, $1, $2, $3);
   if(!findfunc($1) && (ifvardef($1)||ifarraydef($1))) {
            printf("Error type 4 at Line %d:'%s' is not a function.\n ",yylineno,$1->content);
            exit(-1);
    } else if(!findfunc($1)) {
        printf("Error type 5 at Line %d:Undefined function %s\n ",yylineno,$1->content);
            exit(-1);
    } else{
        $$->type = typefunc($1);
    }
   }
   |Exp LB Exp RB {
   $$ = newAst("Exp", 4, $1, $2, $3, $4);
   }
   |Exp GETMEMBER ID {
   $$ = newAst("Exp", 3, $1, $2, $3);
   }
   |ID {
   $$ = newAst("Exp", 1, $1);
   if (!ifvardef($1) && !ifarraydef($1)) {
      printf("Error type 1 at line %d:Undefined Variable %s\n ", yylineno, $1->content);
            exit(-1);
    } else {
      $$->type = typevar($1);
    }
   }
   |INT {
   $$ = newAst("Exp", 1, $1);
   $$->tag = 3;
   $$->type = "int";
   }
   | FLOAT {
   $$ = newAst("Exp", 1, $1);
   $$->tag = 3;
   $$->value = $1->value;
   }
   ;
Args:Exp COMMA Args {
    $$ = newAst("Args", 3, $1, $2, $3);
    }
    |Exp {
    $$=newAst("Args", 1, $1);
    }
    ;
%%
