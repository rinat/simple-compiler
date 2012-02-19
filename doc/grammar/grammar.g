%token_prefix Token_

%token NumberConstant "number constant"
%token StringConstant "string constant"
%token LParen "("
%token RParen ")"
%token LBracket "["
%token RBracket "]"
%token LBrace "{"
%token RBrace "}"
%token Semicolon ";"
%token char "char"
%token break "break"
%token float "float"
%token int "int"
%token double "double"
%token main "main"
%token GT ">"
%token GTE ">="
%token NE "!="
%token Equals "=="
%token LT "<"
%token LTE "<="
%token Id "identifier"
%token Mult "*"
%token Div "/"
%token Comma ","
%token Add "+"
%token Assign "="
%token Sub "-"
%token if "if"
%token else "else"
%token for "for"
%token do "do"
%token while "while"
%token break "break"
%token read "read"
%token write "write"
%token void "void"
%token CharConstant "char constant"

%start program

program ::= global_var_declarations main_function ;

global_var_declarations ::= declaration Semicolon global_var_declarations ;
global_var_declarations ::= ;

variable ::= Id array ;

declaration ::= type variable ;
array ::= LBracket expression RBracket array_next ;
array ::= ;
array_next ::= LBracket expression RBracket ;
array_next ::= ;

block ::= LBrace statements RBrace ;
block ::= Semicolon ;

main_function ::= main LParen RParen block ;

type ::= int ;
type ::= char ;
type ::= float ;
type ::= double ;

statements ::= statement statements ;
statements ::= ;
statement ::= if_statement ;
statement ::= for_statement ;
statement ::= while_statement ;
statement ::= do_while_statement ;
statement ::= assignment_statement Semicolon ;
statement ::= read_statement Semicolon ;
statement ::= write_statement Semicolon ;
statement ::= break_statement Semicolon ;

for_statement ::= for LParen for_part Semicolon  condition Semicolon for_part RParen block ;

while_statement ::= while LParen condition RParen block ;

do_while_statement ::= do block while LParen condition RParen ;

read_statement ::= read LParen read_args RParen ;

write_statement ::= write LParen write_args RParen ;


if_statement ::= if LParen condition RParen block else_block ;

break_statement ::= break ;

condition ::= expression comparison expression ;

else_block ::= else block ;
else_block ::= ;

for_part ::= assignment_statement ;
for_part ::= ;

assignment_statement ::= assignment Comma assignment_statement ;
assignment_statement ::= assignment ;
assignment ::= variable Assign expression ;

read_args ::= variable ;
read_args ::= variable Comma read_args ;

write_args ::= expression ;
write_args ::= expression Comma write_args ;

comparison ::= GT ;
comparison ::= LT ;
comparison ::= GTE ;
comparison ::= LTE ;
comparison ::= NE ;
comparison ::= Equals ;

expression ::= term ;
expression ::= add_expression ;
expression ::= sub_expression ;

add_expression ::= expression Add term ;
sub_expression ::= expression Sub term ;

term ::= unary_expression ;
term ::= mult_expression ;
term ::= div_expression ;

mult_expression ::= unary_expression Mult term ;
div_expression ::= unary_expression Div term ;

unary_expression ::= unary_operator factor ;
unary_expression ::= factor ;

factor ::= variable ;
factor ::= NumberConstant ;
factor ::= LParen expression RParen ;

unary_operator ::= Sub ;

factor ::= StringConstant ;
factor ::= CharConstant ;
