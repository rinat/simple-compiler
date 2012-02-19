/*
* This code is under GNU Lesser General Public License.
* Simple C Compiler to Low Level Virtual Machine representation
*
* Authors: Rinat Shaikhutdinov (aka Sagit) - architecture, compiler core
*          Alexei Zhdanov (aka Alex) - parser
*          Timofey Tatarinov (aka Tikers) - code generator
*
* University Mari State University
* Faculty Informatics and Computer Science
* Course 4
* Specialization PS-41
* Subject Theory of programming languages and translation methods
* Yoshkar-Ola 2010
*/
#ifndef __Frontend_GrammarHelp_h__
#define __Frontend_GrammarHelp_h__

namespace grammar
{
    char g_grammar[] = "    \n\
    \n\
%token_prefix Token_    \n\
    \n\
%token NumberConstant \"number constant\"    \n\
%token StringConstant \"string constant\"    \n\
%token LParen \"(\"    \n\
%token RParen \")\"    \n\
%token LBracket \"[\"    \n\
%token RBracket \"]\"    \n\
%token LBrace \"{\"    \n\
%token RBrace \"}\"    \n\
%token Semicolon \";\"    \n\
%token char \"char\"    \n\
%token break \"break\"    \n\
%token float \"float\"    \n\
%token int \"int\"    \n\
%token double \"double\"    \n\
%token main \"main\"    \n\
%token GT \">\"    \n\
%token GTE \">=\"    \n\
%token NE \"!=\"    \n\
%token Equals \"==\"    \n\
%token LT \"<\"    \n\
%token LTE \"<=\"    \n\
%token Id \"identifier\"    \n\
%token Mult \"*\"    \n\
%token Div \"/\"    \n\
%token Comma \",\"    \n\
%token Add \"+\"    \n\
%token Assign \"=\"    \n\
%token Sub \"-\"    \n\
%token if \"if\"    \n\
%token else \"else\"    \n\
%token for \"for\"    \n\
%token do \"do\"    \n\
%token while \"while\"     \n\
%token break \"break\"    \n\
%token read \"read\"    \n\
%token write \"write\"    \n\
%token void \"void\"    \n\
%token CharConstant \"char constant\"    \n\
    \n\
%start program    \n\
    \n\
program ::= global_var_declarations main_function ;    \n\
    \n\
global_var_declarations ::= declaration Semicolon global_var_declarations ;    \n\
global_var_declarations ::= ;    \n\
    \n\
variable ::= Id array ;    \n\
    \n\
declaration ::= type variable ;    \n\
array ::= LBracket expression RBracket array_next ;    \n\
array ::= ;    \n\
array_next ::= LBracket expression RBracket ;    \n\
array_next ::= ;    \n\
    \n\
block ::= LBrace statements RBrace ;    \n\
block ::= Semicolon ;    \n\
    \n\
main_function ::= main LParen RParen block ;    \n\
    \n\
type ::= int ;    \n\
type ::= char ;    \n\
type ::= float ;    \n\
type ::= double ;    \n\
    \n\
statements ::= statement statements ;    \n\
statements ::= ;    \n\
statement ::= if_statement ;    \n\
statement ::= for_statement ;    \n\
statement ::= while_statement ;    \n\
statement ::= do_while_statement ;    \n\
statement ::= assignment_statement Semicolon ;    \n\
statement ::= read_statement Semicolon ;    \n\
statement ::= write_statement Semicolon ;    \n\
statement ::= break_statement Semicolon ;    \n\
    \n\
for_statement ::= for LParen for_part Semicolon  condition Semicolon for_part RParen block ;    \n\
    \n\
while_statement ::= while LParen condition RParen block ;    \n\
    \n\
do_while_statement ::= do block while LParen condition RParen ;    \n\
    \n\
read_statement ::= read LParen read_args RParen ;    \n\
    \n\
write_statement ::= write LParen write_args RParen ;    \n\
    \n\
    \n\
if_statement ::= if LParen condition RParen block else_block ;    \n\
    \n\
break_statement ::= break ;    \n\
    \n\
condition ::= expression comparison expression ;    \n\
    \n\
else_block ::= else block ;    \n\
else_block ::= ;    \n\
    \n\
for_part ::= assignment_statement ;    \n\
for_part ::= ;    \n\
    \n\
assignment_statement ::= assignment Comma assignment_statement ;    \n\
assignment_statement ::= assignment ;    \n\
assignment ::= variable Assign expression ;    \n\
    \n\
read_args ::= variable ;    \n\
read_args ::= variable Comma read_args ;    \n\
    \n\
write_args ::= expression ;    \n\
write_args ::= expression Comma write_args ;    \n\
    \n\
comparison ::= GT ;    \n\
comparison ::= LT ;    \n\
comparison ::= GTE ;    \n\
comparison ::= LTE ;    \n\
comparison ::= NE ;    \n\
comparison ::= Equals ;    \n\
    \n\
expression ::= term ;    \n\
expression ::= add_expression ;    \n\
expression ::= sub_expression ;    \n\
    \n\
add_expression ::= expression Add term ;    \n\
sub_expression ::= expression Sub term ;    \n\
    \n\
term ::= unary_expression ;    \n\
term ::= mult_expression ;    \n\
term ::= div_expression ;    \n\
    \n\
mult_expression ::= unary_expression Mult term ;    \n\
div_expression ::= unary_expression Div term ;    \n\
    \n\
unary_expression ::= unary_operator factor ;    \n\
unary_expression ::= factor ;    \n\
    \n\
factor ::= variable ;    \n\
factor ::= NumberConstant ;    \n\
factor ::= LParen expression RParen ;    \n\
    \n\
unary_operator ::= Sub ;    \n\
    \n\
factor ::= StringConstant ;    \n\
factor ::= CharConstant ;    \n\
    \n\
";
}

#endif