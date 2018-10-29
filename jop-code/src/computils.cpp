#include <utility>

//
// Created by Joseph on 26/10/2018.
//

#include "computils.h"
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <spdlog/spdlog.h>
#include <boost/format.hpp>
#include <antlr4-runtime/Token.h>

std::string indent(std::string x) {

    std::vector<std::string> lines;
    boost::algorithm::split(lines, x, boost::algorithm::is_any_of("\n"));
    std::string res;
    for (const auto &line : lines) {
        if (!line.empty()) {
            res += "  " + line + "\n";
        }
    }

    return res;

}

template<>
std::string stringify(std::vector<char> v) {
    return std::string(v.begin(), v.end());
}

std::string gen_err_msg(std::string message, size_t lineno, size_t colno, std::string relevantText) {
    boost::format formatter{"%s at line %d:%d, '%s'"};
    formatter % message % lineno % colno % relevantText;
    return formatter.str();
}

void compiler_warn(std::string message, antlr4::ParserRuleContext* token) {
    spdlog::get("console")->warn(gen_err_msg(
            std::move(message), token->getStart()->getLine(),
            token->getStart()->getCharPositionInLine(), token->getText()
    ));
}

void compiler_warn(std::string message, antlr4::tree::TerminalNode* token) {
    spdlog::get("console")->warn(gen_err_msg(
            std::move(message), token->getSymbol()->getLine(),
            token->getSymbol()->getCharPositionInLine(), token->getText()
    ));

}

void compiler_error(std::string message, antlr4::ParserRuleContext* token) {
    const std::string &errMsg = gen_err_msg(
                std::move(message), token->getStart()->getLine(),
                token->getStart()->getCharPositionInLine(), token->getText()
        );
    spdlog::get("console")->error(errMsg);
    throw CompilerError(errMsg);
}

void compiler_error(std::string message, antlr4::tree::TerminalNode* token) {
    const std::string &errMsg = gen_err_msg(
                std::move(message), token->getSymbol()->getLine(),
                token->getSymbol()->getCharPositionInLine(), token->getText()
        );
    spdlog::get("console")->error(errMsg);
    throw CompilerError(errMsg);
}


CompilerError::CompilerError(std::string message) : message_(std::move(message)) {}