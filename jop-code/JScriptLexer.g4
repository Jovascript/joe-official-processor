lexer grammar JScriptLexer;

@members {
int bracket_depth = 0;
}

IDENTIFIER: [\p{Alpha}\p{General_Category=Other_Letter}_] [\p{Alnum}\p{General_Category=Other_Letter}_]*;

STRING: '"' ( '\\"' | ~["\r\n] )* '"'
    | '\'' ( '\\\'' | ~['\r\n] )* '\'';

LINE_COMMENT: ('#' | '//') ~[\r\n]* NEWLINE -> skip;
MULTILINE_COMMENT_BEGIN: '/*' -> skip, mode(COMMENT);


fragment DIGIT: [0-9];
NEWLINE: ('\r'? '\n') {
if (bracket_depth > 0) {
skip();
} else {
size_t la = _input->LA(1);
if ((char)la == '\r' || (char)la == '\n') {
skip();
}
}
};

INTEGER: DIGIT+;


STAR_CHAR: '*';
ADDR_CHAR: '&';

NOT_SIGN: '!';
AND_SIGN: '&&';
OR_SIGN: '||';
TEST_EQ_SIGN: '==';


EQ_SIGN: '=';
GT_SIGN: '>';
LT_SIGN: '<';
MINUS_SIGN: '-';
PLUS_SIGN: '+';
DIVIDE_SIGN: '/';
NEQ_SIGN: '!=' | '<>';
COMMA: ',';

IF_KW: 'if';
WHILE_KW: 'while';


// Cant skip, since it could be a code block...
OPEN_BRACE: '{';
CLOSE_BRACE: '}';
OPEN_BRACKET: '['{ bracket_depth++; };
CLOSE_BRACKET: ']' { bracket_depth--; };

OPEN_PAREN: '(' { bracket_depth++; };
CLOSE_PAREN: ')' { bracket_depth--; };



WS: [ \t] -> skip;

mode COMMENT;
COMMENT_END: '*/' -> skip, mode(DEFAULT_MODE);
TEXT: . -> skip;


