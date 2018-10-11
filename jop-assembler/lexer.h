//
// Created by Joseph on 09/10/2018.
//

#ifndef JOP_LEXER_H
#define JOP_LEXER_H


#include <istream>
#include <memory>
#include <proc_types.h>
#include <boost/optional.hpp>
#include <stack>
#include <queue>
#include "compilation_helpers.h"


struct file_position {
    int loc;
    int line_no;
    int col_no;
};

class LexerError: public CompilerError {
    using CompilerError::CompilerError;

};

struct Token {
    Token();
    explicit Token(const std::string &identifier);
    Token(int number, bool is_reg);


    enum TokenType { tok_identifier, tok_reg,tok_literal, tok_colon, tok_eof} type;


    std::string identifier;
    int literal;
    jop::dtype reg;
    file_position start_pos;
    file_position end_pos;
    std::string parsed_text;
    explicit Token(TokenType type1);
};

class lexer {
public:
    static const char FILE_END = 0x3;

    explicit lexer(std::shared_ptr<std::istream> stream);
    Token next_token;
    void consumeToken();

    boost::optional<Token> parse_token(Token::TokenType type);
    std::string format_expected(const std::string &message);
private:
    std::string get_text_between_pos(file_position start, file_position end);
    void cry(const std::string &message);
    void skip_whitespace();
    void consumeChar();

    char next_char;

    std::shared_ptr<std::istream> _stream;
    file_position current_pos;
};


#endif //JOP_LEXER_H
