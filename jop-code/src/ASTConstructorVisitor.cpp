//
// Created by Joseph on 23/10/2018.
//

#include <boost/lexical_cast.hpp>
#include "ASTConstructorVisitor.h"
#include "ast.h"


antlrcpp::Any ASTConstructorVisitor::visitMain(JScript::JScriptParser::MainContext *ctx) {
    auto block = new Block();
    for (auto &statementparse : ctx->statement()) {

        auto m = unwrap<Statement>(visitStatement(statementparse));

        block->add_statement(m);
    }
    return wrap(block);
}

antlrcpp::Any ASTConstructorVisitor::visitDeclaration(JScript::JScriptParser::DeclarationContext *ctx) {
    if (ctx->value) {
        return wrap(new Declaration(unwrap<Type>(visitType(ctx->type())), ctx->name->getText(), unwrap<Expression>(visitExpression(ctx->value))));
    }
    return wrap(new Declaration(unwrap<Type>(visitType(ctx->type())), ctx->name->getText()));
}

antlrcpp::Any ASTConstructorVisitor::visitAssignment(JScript::JScriptParser::AssignmentContext *ctx) {
    return wrap(new Assignment(ctx->name->getText(), unwrap<Expression>(visitExpression(ctx->value))));
}



antlrcpp::Any ASTConstructorVisitor::visitAtom(JScript::JScriptParser::AtomContext *ctx) {
    if (ctx->STRING() != nullptr) {
        auto str = ctx->STRING()->getText();
        str = str.substr(1, str.length()-2);
        std::vector<char> vec(str.begin(), str.end());
        return wrap(new Literal(vec, new Type(ValueTypes::CHAR, false, static_cast<int>(ctx->STRING()->getText().length()))));
    } else if (ctx->INTEGER() != nullptr) {
        return wrap(new Literal(boost::lexical_cast<int>(ctx->INTEGER()->getText()), new Type(ValueTypes::INT)));
    } else if (ctx->IDENTIFIER()){
        if (ctx->STAR_CHAR()) {
            return wrap(new PtrDereference(new Identifier(ctx->IDENTIFIER()->getText())));
        } else {
            return wrap(new Identifier(ctx->IDENTIFIER()->getText()));
        }
    } else {
        compiler_error("An unexplainable error has occurred", ctx);
    }
}

antlrcpp::Any ASTConstructorVisitor::visitBlock(JScript::JScriptParser::BlockContext *ctx) {
    auto block = new Block();
    for (auto &statementparse : ctx->statement()) {
        block->add_statement(unwrap<Statement>(visitStatement(statementparse)));
    }
    return wrap(block);
}

antlrcpp::Any ASTConstructorVisitor::visitType(JScript::JScriptParser::TypeContext *ctx) {
    auto tn = ctx->IDENTIFIER()->getText();

    ValueTypes type;
    if (tn == "int") type = ValueTypes ::INT;
    else if (tn == "uint") type = ValueTypes ::UINT;
    else if (tn == "bool") type = ValueTypes ::BOOL;
    else if (tn == "char") type = ValueTypes ::CHAR;
    else {
        // TODO: Add error handling
        compiler_error("Unknown type", ctx->IDENTIFIER());
    }
    return wrap(new Type(type, ctx->pointer != nullptr));
}

antlrcpp::Any ASTConstructorVisitor::visitIfStmt(JScript::JScriptParser::IfStmtContext *ctx) {
    if (ctx->ELSE_KW()) {
        return wrap(new IFStatement(unwrap<Expression>(visitExpression(ctx->expression())), unwrap<Block>(visitBlock(ctx->block()[0])), unwrap<Block>(visitBlock(ctx->block()[1]))));
    } else {
        return wrap(new IFStatement(unwrap<Expression>(visitExpression(ctx->expression())), unwrap<Block>(visitBlock(ctx->block()[0]))));
    }
}

antlrcpp::Any ASTConstructorVisitor::visitWhileStmt(JScript::JScriptParser::WhileStmtContext *ctx) {
    return wrap(new WhileStatement(unwrap<Expression>(visitExpression(ctx->expression())), unwrap<Block>(visitBlock(ctx->block()))));
}

antlrcpp::Any ASTConstructorVisitor::visitGroupedExpr(JScript::JScriptParser::GroupedExprContext *ctx) {
    return visitExpression(ctx->expression());
}

antlrcpp::Any ASTConstructorVisitor::visitProcCallExpr(JScript::JScriptParser::ProcCallExprContext *ctx) {
    return JScriptParserBaseVisitor::visitProcCallExpr(ctx);
}

antlrcpp::Any ASTConstructorVisitor::visitBinaryOp(JScript::JScriptParser::BinaryOpContext *ctx) {
    auto x = ctx->s->getText();
    BinaryOperation op;
    if (x=="*") op = BINOP_MUL;
    else if (x=="/") op = BINOP_DIV;
    else if (x=="+") op = BINOP_ADD;
    else if (x=="-") op = BINOP_SUB;
    else if (x=="==") op = BINOP_EQ;
    else if (x=="!=") op = BINOP_NEQ;
    else if (x==">") op = BINOP_GT;
    else if (x=="<") op = BINOP_LT;
    else if (x=="&&") op = BINOP_AND;
    else if (x=="||") op = BINOP_OR;
    else {/*?????*/}
    return wrap(new BinaryExpression(unwrap<Expression>(visitExpression(ctx->expression()[0])), op, unwrap<Expression>(visitExpression(ctx->expression()[1]))));
}

antlrcpp::Any ASTConstructorVisitor::visitAtomExpr(JScript::JScriptParser::AtomExprContext *ctx) {
    return visitAtom(ctx->atom());
}

antlrcpp::Any ASTConstructorVisitor::visitUnaryOp(JScript::JScriptParser::UnaryOpContext *ctx) {
    auto op = ctx->s->getText() == "!" ? UNOP_NOT : UNOP_NEG;
    return wrap(new UnaryExpression(op, unwrap<Expression>(visitExpression(ctx->expression()))));
}

antlrcpp::Any ASTConstructorVisitor::visitExpression(JScript::JScriptParser::ExpressionContext *ctx) {
    return ctx->accept(this);
}

antlrcpp::Any ASTConstructorVisitor::visitStatement(JScript::JScriptParser::StatementContext *ctx) {
    return ctx->children[0]->accept(this);
}

NodeWrapper wrap(ASTNode* val) {
    return NodeWrapper(val);
}
