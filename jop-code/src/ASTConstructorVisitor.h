//
// Created by Joseph on 23/10/2018.
//

#ifndef JOP_ASTCONSTRUCTORVISITOR_H
#define JOP_ASTCONSTRUCTORVISITOR_H

#include "JScriptParserBaseVisitor.h"
class ASTNode;

class NodeWrapper {
public:
    explicit NodeWrapper(ASTNode* value) : val(value) {}

    ASTNode* val;
};

NodeWrapper wrap(ASTNode* val);

template<class T>
T* unwrap(NodeWrapper wrapper) {
    if (auto x = dynamic_cast<T*>(wrapper.val)) {
        return x;
    } else {
        throw std::bad_cast();
    }
}

template<class T>
T* unwrap(antlrcpp::Any wrapper) {
    NodeWrapper x = wrapper;
    return unwrap<T>(x);
}

class ASTConstructorVisitor: public JScript::JScriptParserBaseVisitor {
public:
    antlrcpp::Any visitMain(JScript::JScriptParser::MainContext *ctx) override;

    antlrcpp::Any visitDeclaration(JScript::JScriptParser::DeclarationContext *ctx) override;

    antlrcpp::Any visitAssignment(JScript::JScriptParser::AssignmentContext *ctx) override;

    antlrcpp::Any visitAtom(JScript::JScriptParser::AtomContext *ctx) override;

    antlrcpp::Any visitBlock(JScript::JScriptParser::BlockContext *ctx) override;

    antlrcpp::Any visitType(JScript::JScriptParser::TypeContext *ctx) override;

    antlrcpp::Any visitIfStmt(JScript::JScriptParser::IfStmtContext *ctx) override;

    antlrcpp::Any visitWhileStmt(JScript::JScriptParser::WhileStmtContext *ctx) override;

    antlrcpp::Any visitGroupedExpr(JScript::JScriptParser::GroupedExprContext *ctx) override;

    antlrcpp::Any visitProcCallExpr(JScript::JScriptParser::ProcCallExprContext *ctx) override;

    antlrcpp::Any visitBinaryOp(JScript::JScriptParser::BinaryOpContext *ctx) override;

    antlrcpp::Any visitAtomExpr(JScript::JScriptParser::AtomExprContext *ctx) override;

    antlrcpp::Any visitUnaryOp(JScript::JScriptParser::UnaryOpContext *ctx) override;

    antlrcpp::Any visitExpression(JScript::JScriptParser::ExpressionContext* ctx);

    antlrcpp::Any visitStatement(JScript::JScriptParser::StatementContext *ctx) override;


};


#endif //JOP_ASTCONSTRUCTORVISITOR_H
