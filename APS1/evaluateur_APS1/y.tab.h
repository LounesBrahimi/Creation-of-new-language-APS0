/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CONST = 258,
    FUN = 259,
    REC = 260,
    NUM = 261,
    BOOL = 262,
    IDENT = 263,
    INT = 264,
    TBOOL = 265,
    ECHO = 266,
    LSQBR = 267,
    RSQBR = 268,
    LPAR = 269,
    RPAR = 270,
    STAR = 271,
    ARROW = 272,
    DDOTS = 273,
    COMMA = 274,
    SEMICOLON = 275,
    IF = 276,
    AND = 277,
    OR = 278,
    ADD = 279,
    MUL = 280,
    EQ = 281,
    SUB = 282,
    DIV = 283,
    LT = 284,
    NOT = 285,
    VAR = 286,
    PROC = 287,
    SET = 288,
    IFB = 289,
    WHILE = 290,
    CALL = 291
  };
#endif
/* Tokens.  */
#define CONST 258
#define FUN 259
#define REC 260
#define NUM 261
#define BOOL 262
#define IDENT 263
#define INT 264
#define TBOOL 265
#define ECHO 266
#define LSQBR 267
#define RSQBR 268
#define LPAR 269
#define RPAR 270
#define STAR 271
#define ARROW 272
#define DDOTS 273
#define COMMA 274
#define SEMICOLON 275
#define IF 276
#define AND 277
#define OR 278
#define ADD 279
#define MUL 280
#define EQ 281
#define SUB 282
#define DIV 283
#define LT 284
#define NOT 285
#define VAR 286
#define PROC 287
#define SET 288
#define IFB 289
#define WHILE 290
#define CALL 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 66 "parser.y" /* yacc.c:1909  */

  int num;
  char* str;
  Sexpr expr;
  Sexprs exprs;
  def def_;
  arg arg_;
  type type_;
  prog* block;
  stat stat_;

#line 138 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
