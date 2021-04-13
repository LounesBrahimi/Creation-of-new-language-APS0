/* ========================================================================== */
/* == UPMC/master/info/4I506 -- Janvier 2016/2017/2018                     == */
/* == SU/FSI/master/info/MU4IN503 -- Janvier 2020/2021                     == */
/* == Analyse des programmes et sémantiques                                == */
/* ========================================================================== */
/* == S-expressions Syntaxe C                                              == */
/* == Fichier: lexer.lex                                                   == */
/* == Lexique                                                              == */
/* ========================================================================== */

%{

#include <stdlib.h>

#include "ast.h"  
#include "y.tab.h"


%}
boolean true|false
const CONST
fun   FUN
rec   REC
type_int	int
echo ECHO
nls "\n"|"\r"|"\r\n"
nums "-"?[0-9]+
idents [a-zA-Z][a-zA-Z0-9]*
%%

[ \t\n\r]  { /* on ignore */ }
"\r\n"     { /* on ignore aussi */ }

"#".*

"["   { return(LSQBR);}
"]"   { return(RSQBR);}

"("   { return(LPAR); }
")"   { return(RPAR); }

"*"    { return(STAR);}
"->"   { return(ARROW);}

":"   { return(DDOTS);}
","   { return(COMMA);}

";"	  { return(SEMICOLON);}

"bool" {
		    yylval.str=strdup(yytext);
            return(TBOOL);	
        }

"int"  {
		    yylval.str=strdup(yytext);
            return(INT);	
		}

"if"  { return (IF); }

"and"  { return (AND); }

"or"  { return (OR); }

"add"  { return (ADD); }

"mul"  { return (MUL); }

"eq"  { return (EQ); }

"sub"  { return (SUB); }

"div"  { return (DIV); }

"lt"  { return (LT); }

"not"  { return (NOT); }

{nums}    {
            yylval.num=atoi(yytext);
            return(NUM);
          }

{boolean}  {
            yylval.str=strdup(yytext);
            return(BOOL);
          }
          
{const}  {
            yylval.str=strdup(yytext);
            return(CONST);
          }

{fun}  {
            yylval.str=strdup(yytext);
            return(FUN);
          }

{rec}  {
            yylval.str=strdup(yytext);
            return(REC);
          }

{echo}  {
            yylval.str=strdup(yytext);
            return(ECHO);
          }

{idents}  {
            yylval.str=strdup(yytext);
            return(IDENT);
          }
