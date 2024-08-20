/* -------------------------------------------------------
   The grammar symbols
   ------------------------------------------------------- */
%token	<string_val> WORD

%token 	NOTOKEN NEWLINE

%union	{
			char   *string_val;
		}

/* The following are the operators (see XBD Operator)
   containing more than one character. */

%token  PIPE
/*      '|'    */

%token  DLESS  DGREAT  LESS  GREAT
/*      '<<'   '>>'    '<'   '>'     */

%{
#include <stdio.h>

void yyerror(const char * s);
int yylex();
%}

/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */
%start pipe_sequence
%%
pipe_sequence	: simple_command
				| pipe_sequence PIPE simple_command
				;
simple_command	: cmd_prefix cmd_word cmd_suffix
				| cmd_prefix cmd_word
				| cmd_prefix
				| cmd_name cmd_suffix
				| cmd_name
				;
cmd_name		: WORD
				;
cmd_word		: WORD
				;
cmd_prefix		: io_redirect
				| cmd_prefix io_redirect
				;
cmd_suffix		: io_redirect
				| cmd_suffix io_redirect
				| WORD
				| cmd_suffix WORD
				;
io_redirect		: io_file
				| io_here
				;
io_file			: GREAT  filename
				| LESS   filename
				| DGREAT filename
				;
filename		: WORD
				;
io_here			: DLESS here_end
				;
here_end		: WORD
				;
%%

void yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

void prompt()
{
    printf("shell> ");
}
