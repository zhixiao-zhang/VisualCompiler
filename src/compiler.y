%{
#include <unistd.h>
#include <stdio.h>
#include <../include/compiler.h>
%}

%union {
  pnode type_pnode;
  double d;
}

 /*声明记号的值*/
%token <type_pnode> INT
%token <type_pnode> TYPE STRUCT RETURN IF ELSE WHILE ID COMMENT SPACE SEMI COMMA ASSIGNOP PLUS
%token <type_pnode> MINUS STAR DIV AND OR GETMEMEBER NOT LP RP LB RB LC RC AERROR RELOP EOL
%token <type_pnode> Program ExtDefList ExtDef ExtDecList Specifire StructSpecifire
%token <type_pnode> OptTag Tag VarDec FuncDeclare VarList ParamDeclare Compst StmtList Stmt DefList Defition DecList Dec Exp Args

 /*规定优先级*/
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB GETMEMEBER

 /*伪记号*/
%nonassoc _LOWER_THAN_ELSE

%nonassoc ELSE

 /*产生式*/
%%
 /*高级定义*/
Program:ExtDefList {
       $$ = newAst("Program", 1, $1);
       nodeList[_nodeNum] = $$;
       _nodeNum++;
       }
       ;
ExtDefList:ExtDef ExtDefList {
          $$ = newAst("ExtDefList", 2, $1, $2);
          nodeList[_nodeNum] = $$;
          _nodeNum++;
          } 
          | {
          $$ = newAst("ExtDefList", 0, -1);
          nodeList[_nodeNum] = $$;
          _nodeNum++;
          }
          ;
ExtDef:Specifire ExtDecList SEMI {
      $$ = newAst("ExtDef", 3, $1, $2, $3);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      |Specifire SEMI {
      $$ = newAst("ExtDef", 2, $1, $2);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      |Specifire FuncDeclare Compst {
      $$ = newAst("ExtDef", 3, $1, $2, $3);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      ;
ExtDecList:VarDec {
          $$ = newAst("ExtDecList", 1, $1);
          nodeList[_nodeNum] = $$;
          _nodeNum++;
          }
          |VarDec COMMA ExtDecList {
          $$ = newAst("ExtDecList", 3, $1, $2, $3);
          nodeList[_nodeNum] = $$;
          _nodeNum++;
          }
          ;

 /*说明符*/
Specifire:TYPE {
         $$ = newAst("Specifire", 1, $1);
         nodeList[_nodeNum] = $$;
         _nodeNum++;
         }
         | StructSpecifire {
         $$ = newAst("StructSpecifire", 1, $1);
         nodeList[_nodeNum] = $$;
         _nodeNum++;
         }
         ;
StructSpecifire:STRUCT OptTag LC DefList RC {
               $$ = newAst("StructSpecifire", 5, $1, $2, $3, $4 , $5);
               nodeList[_nodeNum] = $$;
               _nodeNum++;
               }
               |STRUCT Tag {
               $$ = newAst("StructSpecifire", 2, $1, $2);
               nodeList[_nodeNum] = $$;
               _nodeNum++;
               }
               ;
OptTag:ID {
      $$ = newAst("OptTag", 1, $1);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      | {
      $$ = newAst("OptTag", 0, -1);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      ;
Tag:ID {
   $$ = newAst("Tag", 1, $1);
   nodeList[_nodeNum] = $$;
   _nodeNum++;
   }
   ;

 /*声明符*/
VarDec:ID {
      $$ = newAst("VarDec", 1, $1);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      |VarDec LB INT RB {
      $$ = newAst("VarDec", 4, $1, $2, $3, $4);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      ;
FuncDeclare:ID LP VarList RP {
           $$ = newAst("FuncDeclare", 4, $1, $2, $3, $4);
           nodeList[_nodeNum] = $$;
           _nodeNum++;
           }
           |ID LP RP {
           $$ = newAst("FuncDeclare", 3, $1, $2, $3);
           nodeList[_nodeNum] = $$;
           _nodeNum++;
           }
           ;
VarList:ParamDeclare COMMA VarList {
       $$ = newAst("FuncDeclare", 3, $1, $2, $3);
       nodeList[_nodeNum] = $$;
       _nodeNum++;
       }
       |ParamDeclare {
       $$ = newAst("VarList", 1, $1);
       nodeList[_nodeNum] = $$;
       _nodeNum++;
       }
       ;
ParamDeclare:Specifire VarDec {
            $$ = newAst("ParamDeclare", 2, $1, $2);
            nodeList[_nodeNum] = $$;
            _nodeNum++;
            }
            ;

 /*声明*/
Compst:LC DefList StmtList RC {
      $$ = newAst("Compst", 4, $1, $2, $3, $4);
      nodeList[_nodeNum] = $$;
      _nodeNum++;
      }
      ;
StmtList:Stmt StmtList {
        $$ = newAst("StmtList", 2, $1, $2);
        nodeList[_nodeNum] = $$;
        _nodeNum++;
        }
        | {
        $$ = newAst("StmtList", 0, -1);
        nodeList[_nodeNum] = $$;
        _nodeNum++;
        }
        ;
Stmt:Exp SEMI {
    $$ = newAst("Stmt", 2, $1, $2);
    nodeList[_nodeNum] = $$;
    _nodeNum++;
    }
