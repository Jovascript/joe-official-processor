parser grammar JScriptParser;

options {
tokenVocab=JScriptLexer;
}

main: (NEWLINE | statement)* EOF;

statement: (declaration
    | assignment
    | if_stmt
    | while_stmt
//   | for_stmt
    | proc_call
    | proc_def
    | proc_decl) (NEWLINE | EOF);

declaration: type name=IDENTIFIER ('=' value=expression)?;

assignment: name=IDENTIFIER '=' value=expression;

proc_call: IDENTIFIER '(' arglist ')';

proc_decl: ret_type=IDENTIFIER name=IDENTIFIER '(' typed_arglist ')';

proc_def: proc_decl block;

if_stmt: 'if' '(' expression ')' block;
while_stmt: 'while' '(' expression ')' block;

expression:
    proc_call # proc_call_expr
    | '!' expression # binary_not
    | expression ('*' | '/') expression # muldiv_op
    | expression ('+' | '-') expression # addsub_op
    | expression ('==' | NEQ_SIGN) expression # eqneq_op
    | expression '>' expression # gt_op
    | expression '<' expression # lt_op
    | expression '&&' expression # binary_and
    | expression '||' expression # binary_or
    | '(' expression ')' # grouped_expr
    | atom # atom_expr
;

atom:
    '*' IDENTIFIER
    | IDENTIFIER
    | STRING
    | INTEGER;

block: '{' (NEWLINE | statement)* '}';

arglist: (atom (',' atom)*)?;
typed_arglist: (type IDENTIFIER (',' type IDENTIFIER)*)?;

type: IDENTIFIER pointer='*'?;

