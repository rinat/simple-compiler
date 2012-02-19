%parser         Grammar
%decl           Parser.h
%impl           Parser.cpp

%token_prefix Token_

%token NumberConstant "number constant"
%token StringConstant "string constant"
%token CharacterConstant "Character constant"

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

%start program

program ::= global_var_declarations main_function ;
/.
case $rule_number: {
  // program ::= global_var_declarations main_function ;
} break;
./

global_var_declarations ::= declaration Semicolon global_var_declarations ;
/.
case $rule_number: {
  // global_var_declarations ::= declaration Semicolon global_var_declarations ;
} break;
./

global_var_declarations ::= ;
/.
case $rule_number: {
  // global_var_declarations ::= ;
} break;
./

variable ::= Id array ;
/.
case $rule_number: {
  // variable ::= Id array ;
} break;
./

declaration ::= type variable ;
/.
case $rule_number: {
  // declaration ::= type variable ;
} break;
./

array ::= LBracket expression RBracket array_next ;
/.
case $rule_number: {
  // array ::= LBracket expression RBracket array_next ;
} break;
./

array ::= ;
/.
case $rule_number: {
  // array ::= ;
} break;
./

array_next ::= LBracket expression RBracket ;
/.
case $rule_number: {
  // array_next ::= LBracket expression RBracket ;
} break;
./

array_next ::= ;
/.
case $rule_number: {
  // array_next ::= ;
} break;
./

block ::= LBrace statements RBrace ;
/.
case $rule_number: {
  // block ::= LBrace statements RBrace ;
} break;
./

block ::= Semicolon ;
/.
case $rule_number: {
  // block ::= Semicolon ;
} break;
./

main_function ::= main LParen RParen block ;
/.
case $rule_number: {
  // main_function ::= main LParen RParen block ;
} break;
./

type ::= int ;
/.
case $rule_number: {
  // type ::= int ;
} break;
./

type ::= char ;
/.
case $rule_number: {
  // type ::= char ;
} break;
./

type ::= float ;
/.
case $rule_number: {
  // type ::= float ;
} break;
./

type ::= double ;
/.
case $rule_number: {
  // type ::= double ;
} break;
./

statements ::= statement statements ;
/.
case $rule_number: {
  // statements ::= statement statements ;
} break;
./

statements ::= ;
/.
case $rule_number: {
  // statements ::= ;
} break;
./

statement ::= if_statement ;
/.
case $rule_number: {
  // statement ::= if_statement ;
} break;
./

statement ::= for_statement ;
/.
case $rule_number: {
  // statement ::= for_statement ;
} break;
./

statement ::= while_statement ;
/.
case $rule_number: {
  // statement ::= while_statement ;
} break;
./

statement ::= do_while_statement ;
/.
case $rule_number: {
  // statement ::= do_while_statement ;
} break;
./

statement ::= assignment_statement Semicolon ;
/.
case $rule_number: {
  // statement ::= assignment_statement Semicolon ;
} break;
./

statement ::= read_statement Semicolon ;
/.
case $rule_number: {
  // statement ::= read_statement Semicolon ;
} break;
./

statement ::= write_statement Semicolon ;
/.
case $rule_number: {
  // statement ::= write_statement Semicolon ;
} break;
./

statement ::= break_statement Semicolon ;
/.
case $rule_number: {
  // statement ::= break_statement Semicolon ;
} break;
./

for_statement ::= for LParen for_part Semicolon  condition Semicolon for_part RParen block ;
/.
case $rule_number: {
  // for_statement ::= for LParen for_part Semicolon  condition Semicolon for_part RParen block ;
} break;
./

while_statement ::= while LParen condition RParen block ;
/.
case $rule_number: {
  // while_statement ::= while LParen condition RParen block ;
} break;
./

do_while_statement ::= do block while LParen condition RParen ;
/.
case $rule_number: {
  // do_while_statement ::= do block while LParen condition RParen ;
} break;
./

read_statement ::= read LParen read_args RParen ;
/.
case $rule_number: {
  // read_statement ::= read LParen read_args RParen ;
} break;
./

write_statement ::= write LParen write_args RParen ;
/.
case $rule_number: {
  // write_statement ::= write LParen write_args RParen ;
} break;
./

if_statement ::= if LParen condition RParen block else_block ;
/.
case $rule_number: {
  // if_statement ::= if LParen condition RParen block else_block ;
} break;
./

break_statement ::= break ;
/.
case $rule_number: {
  // break_statement ::= break ;
} break;
./

condition ::= expression comparison expression ;
/.
case $rule_number: {
  // condition ::= expression comparison expression ;
} break;
./

else_block ::= else block ;
/.
case $rule_number: {
  // else_block ::= else block ;
} break;
./

else_block ::= ;
/.
case $rule_number: {
  // else_block ::= ;
} break;
./

for_part ::= assignment_statement ;
/.
case $rule_number: {
  // for_part ::= assignment_statement ;
} break;
./

for_part ::= ;
/.
case $rule_number: {
  // for_part ::= ;
} break;
./

assignment_statement ::= assignment Comma assignment_statement ;
/.
case $rule_number: {
  // assignment_statement ::= assignment Comma assignment_statement ;
} break;
./

assignment_statement ::= assignment ;
/.
case $rule_number: {
  // assignment_statement ::= assignment ;
} break;
./

assignment ::= variable Assign assignment_part ;
/.
case $rule_number: {
  // assignment ::= variable Assign expression ;
} break;
./

assignment_part ::= expression ;
/.
case $rule_number: {
  // assignment_part ::= Assign expression ;
} break;
./

assignment_part ::= primary_expression ;
/.
case $rule_number: {
  // assignment_part ::= Assign StringConstant ;
} break;
./

primary_expression ::= CharacterConstant ;
/.
case $rule_number: {
  // primary_expression ::= CharacterConstant ;
} break;
./

primary_expression ::= StringConstant ;
/.
case $rule_number: {
  // primary_expression ::= StringConstant ;
} break;
./

read_args ::= variable ;
/.
case $rule_number: {
  // read_args ::= variable ;
} break;
./

read_args ::= variable Comma read_args ;
/.
case $rule_number: {
  // read_args ::= variable Comma read_args ;
} break;
./

write_arg ::= variable ;
/.
case $rule_number: {
  // write_arg ::= variable ;
} break;
./

write_arg ::= StringConstant ;
/.
case $rule_number: {
  // write_arg ::= StringConstant ;
} break;
./

write_arg ::= NumberConstant ;
/.
case $rule_number: {
  // write_arg ::= NumberConstant ;
} break;
./

write_arg ::= CharacterConstant ;
/.
case $rule_number: {
  // write_arg ::= CharacterConstant ;
} break;
./


write_args ::= write_arg ;
/.
case $rule_number: {
  // write_args ::= write_arg ;
} break;
./

write_args ::= write_arg Comma write_args ;
/.
case $rule_number: {
  // write_args ::= write_arg Comma write_args ;
} break;
./

comparison ::= GT ;
/.
case $rule_number: {
  // comparison ::= GT ;
} break;
./

comparison ::= LT ;
/.
case $rule_number: {
  // comparison ::= LT ;
} break;
./

comparison ::= GTE ;
/.
case $rule_number: {
  // comparison ::= GTE ;
} break;
./

comparison ::= LTE ;
/.
case $rule_number: {
  // comparison ::= LTE ;
} break;
./

comparison ::= NE ;
/.
case $rule_number: {
  // comparison ::= NE ;
} break;
./

comparison ::= Equals ;
/.
case $rule_number: {
  // comparison ::= Equals ;
} break;
./

expression ::= term ;
/.
case $rule_number: {
  // expression ::= term ;
} break;
./

expression ::= add_expression ;
/.
case $rule_number: {
  // expression ::= add_expression ;
} break;
./

expression ::= sub_expression ;
/.
case $rule_number: {
  // expression ::= sub_expression ;
} break;
./

add_expression ::= term Add expression ;
/.
case $rule_number: {
  // add_expression ::= term Add expression ;
} break;
./

sub_expression ::= term Sub expression ;
/.
case $rule_number: {
  // sub_expression ::= term Sub expression ;
} break;
./

term ::= unary_expression ;
/.
case $rule_number: {
  // term ::= unary_expression ;
} break;
./

term ::= mult_expression ;
/.
case $rule_number: {
  // term ::= mult_expression ;
} break;
./

term ::= div_expression ;
/.
case $rule_number: {
  // term ::= div_expression ;
} break;
./

mult_expression ::= unary_expression Mult term ;
/.
case $rule_number: {
  // mult_expression ::= unary_expression Mult term ;
} break;
./

div_expression ::= unary_expression Div term ;
/.
case $rule_number: {
  // div_expression ::= unary_expression Div term ;
} break;
./

unary_expression ::= unary_operator factor ;
/.
case $rule_number: {
  // unary_expression ::= unary_operator factor ;
} break;
./

unary_expression ::= factor ;
/.
case $rule_number: {
  // unary_expression ::= factor ;
} break;
./

factor ::= variable ;
/.
case $rule_number: {
  // factor ::= variable ;
} break;
./

factor ::= NumberConstant ;
/.
case $rule_number: {
  // factor ::= NumberConstant ;
} break;
./

factor ::= LParen expression RParen ;
/.
case $rule_number: {
  // factor ::= LParen expression RParen ;
} break;
./

unary_operator ::= Sub ;
/.
case $rule_number: {
  // unary_operator ::= Sub ;
} break;
./
