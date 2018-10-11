//
// Created by Joseph on 09/10/2018.
//

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "lexer.h"

static const std::vector<std::string> registers{"A", "B", "C", "D", "E", "F", "G", "PC"};

Token::Token(const std::string &identifier) : identifier(identifier) {
    type = tok_identifier;

}

Token::Token(int number, bool is_reg) {
    if (is_reg) {
        type = tok_reg;
        reg = static_cast<jop::dtype>(number);
    } else {
        type = tok_literal;
        literal = number;
    }
}

Token::Token() : type(tok_eof) {}

Token::Token(Token::TokenType type1) : type(type1) {}

lexer::lexer(std::shared_ptr<std::istream> stream) : current_pos({0, 1, 1}) {
    _stream = std::move(stream);
    consumeChar();
}

std::string lexer::format_expected(const std::string &message) {
    std::string curr_char = next_char == FILE_END ? "<EOF>" : std::string(1, next_char);
    boost::format fstring("%s, got '%s' at %u:%u");
    fstring % message % curr_char % current_pos.line_no % current_pos.col_no;
    return fstring.str();
}

void lexer::consumeToken() {
    skip_whitespace();
    auto start_pos = current_pos;
    if (next_char == '#' or next_char == '0') {
        std::string numberS;
        if (next_char == '#') {
            consumeChar();
            while (isdigit(next_char)) {
                numberS += next_char;
                consumeChar();
            }
        } else {
            // next_char == '0';
            consumeChar();
            if (next_char == 'x' or next_char == 'X') {
                consumeChar();
                while (isxdigit(next_char)) {
                    numberS += next_char;
                    consumeChar();
                }
            } else {
                cry("Expected 'x' or 'X'");
            }
        }
        if (!numberS.empty()) {

            errno = 0;
            long x = strtol(numberS.c_str(), nullptr, 10);
            // Since we did the digits ourselves, it is guaranteed to have produced a number.
            if (errno == ERANGE or x > INT_MAX) {
                cry("Number too large");
            } else {
                next_token = Token(static_cast<int>(x), false);
            }
        } else {
            cry("Expected digit");
        }
    } else if (isalpha(next_char) or next_char == '.' or next_char == '_') {
        // Identifier or register
        std::string id;
        id += next_char;
        consumeChar();
        while (isalnum(next_char) or next_char == '_') {
            id += next_char;
            consumeChar();
        }

        // Looking for a register
        std::string reg = id;
        boost::to_upper(reg);
        // Find in list of registers
        auto it = std::find(registers.begin(), registers.end(), reg);
        if (it != registers.end()) {
            // A register
            next_token = Token(static_cast<int>(it - registers.begin()), true);
        } else {
            // Not a register
            next_token = Token(id);
        }
    } else if (next_char == ':') {
        consumeChar();

        next_token = Token(Token::tok_colon);

    } else if (next_char == FILE_END) {
        // Don't consume char.
        next_token = Token(Token::tok_eof);
    } else {
        cry("Unexpected character");
    }
    next_token.start_pos = start_pos;
    next_token.end_pos = current_pos;
    next_token.parsed_text = get_text_between_pos(start_pos, current_pos);

}

void lexer::skip_whitespace() {
    // You can insert commas anywhere for fun!
    while (isspace(next_char) or next_char == ',') {
        consumeChar();
    }
}

void lexer::cry(const std::string &message) {
    throw LexerError(format_expected(message));

}

void lexer::consumeChar() {
    int c = _stream->get();
    if (c == EOF) {
        next_char = FILE_END;
    } else {
        next_char = static_cast<char>(c);
        current_pos.col_no++;
        current_pos.loc = static_cast<int>(_stream->tellg());

        if (next_char == '\n') {
            current_pos.line_no++;
            current_pos.col_no = 1;
        }
    }
}


std::string lexer::get_text_between_pos(file_position start, file_position end) {
    auto length = static_cast<unsigned long>(end.loc - start.loc);
    _stream->seekg(start.loc);

    std::string text(length, ' ');
    _stream->read(&text[0], length);

    _stream->seekg(current_pos.loc);
    return text;
}

boost::optional<Token> lexer::parse_token(Token::TokenType type) {
    if (next_token.type == type) {
        auto result = next_token;
        consumeToken();
        return result;
    } else {
        return boost::none;
    }
}

