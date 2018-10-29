parser grammar JScriptParser;

options {
tokenVocab=JScriptLexer;
}

main: (NEWLINE | statement)* EOF;

statement: (declaration
    | assignment
    | ifStmt
    | whileStmt
//   | forStmt
/*    | procCall
    | procDef
    | procDecl*/) (NEWLINE | EOF);

declaration: type name=IDENTIFIER ('=' value=expression)?;

assignment: name=IDENTIFIER '=' value=expression;

procCall: IDENTIFIER '(' arglist ')';

procDecl: ret_type=IDENTIFIER name=IDENTIFIER '(' typedArglist ')';

procDef: procDecl block;

ifStmt: IF_KW '(' expression ')' block (ELSE_KW block)?;
whileStmt: 'while' '(' expression ')' block;



expression:
    procCall # procCallExpr
    | s=('!' | '-') expression # unaryOp
    | expression s=('*' | '/') expression # binaryOp
    | expression s=('+' | '-') expression # binaryOp
    | expression s=('==' | NEQ_SIGN) expression # binaryOp
    | expression s='>' expression # binaryOp
    | expression s='<' expression # binaryOp
    | expression s='&&' expression # binaryOp
    | expression s='||' expression # binaryOp
    | '(' expression ')' # groupedExpr
    | atom # atomExpr
;

atom:
    STAR_CHAR IDENTIFIER
    | IDENTIFIER
    | STRING
    | INTEGER;

block: '{' (NEWLINE | statement)* '}';

arglist: (atom (',' atom)*)?;
typedArglist: (type IDENTIFIER (',' type IDENTIFIER)*)?;

type: IDENTIFIER pointer='*'?;

