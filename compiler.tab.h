/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     FLOAT = 259,
     TYPE = 260,
     STRUCT = 261,
     RETURN = 262,
     IF = 263,
     ELSE = 264,
     WHILE = 265,
     ID = 266,
     COMMENT = 267,
     SPACE = 268,
     SEMI = 269,
     COMMA = 270,
     ASSIGNOP = 271,
     PLUS = 272,
     MINUS = 273,
     STAR = 274,
     DIV = 275,
     AND = 276,
     OR = 277,
     GETMEMBER = 278,
     NOT = 279,
     LP = 280,
     RP = 281,
     LB = 282,
     RB = 283,
     LC = 284,
     RC = 285,
     AERROR = 286,
     RELOP = 287,
     EOL = 288,
     LOWER_THAN_ELSE = 289
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define TYPE 260
#define STRUCT 261
#define RETURN 262
#define IF 263
#define ELSE 264
#define WHILE 265
#define ID 266
#define COMMENT 267
#define SPACE 268
#define SEMI 269
#define COMMA 270
#define ASSIGNOP 271
#define PLUS 272
#define MINUS 273
#define STAR 274
#define DIV 275
#define AND 276
#define OR 277
#define GETMEMBER 278
#define NOT 279
#define LP 280
#define RP 281
#define LB 282
#define RB 283
#define LC 284
#define RC 285
#define AERROR 286
#define RELOP 287
#define EOL 288
#define LOWER_THAN_ELSE 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 7 "./src/compiler.y"
{
pnode type_pnode;
double d;
}
/* Line 1529 of yacc.c.  */
#line 122 "compiler.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

