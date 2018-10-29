//
// Created by Joseph on 21/10/2018.
//

#ifndef JOP_AST_H
#define JOP_AST_H

#include <bits/unique_ptr.h>
#include <vector>
#include <boost/format.hpp>
#include <sstream>
#include <unordered_map>
#include <boost/any.hpp>
#include "computils.h"


class ASTVisitor;

enum ValueTypes {
    INT, UINT, CHAR, BOOL
};

static std::unordered_map<ValueTypes, std::string> type_names{{INT, "int"}, {UINT, "uint"}, {CHAR, "char"}, {BOOL, "bool"}};

enum BinaryOperation {
    BINOP_ADD, BINOP_SUB, BINOP_DIV, BINOP_MUL, BINOP_EQ, BINOP_NEQ, BINOP_GT, BINOP_LT, BINOP_AND, BINOP_OR
};

static std::unordered_map<BinaryOperation, std::string> binop_symbols{
    {BINOP_AND, "&&"}, {BINOP_NEQ, "!="}, {BINOP_EQ, "=="}, {BINOP_GT, ">"}, {BINOP_LT, "<"}, {BINOP_OR, "||"},
    {BINOP_ADD, "+"}, {BINOP_SUB, "-"}, {BINOP_MUL, "*"}, {BINOP_DIV, "/"}
};

enum UnaryOperation {
    UNOP_NOT, UNOP_NEG
};

static std::unordered_map<UnaryOperation, std::string> unop_symbols{ {UNOP_NEG, "-"}, {UNOP_NOT, "!"} };

struct ASTNode {
    virtual std::string toString() = 0;
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor &v) = 0;
};

struct Type: public ASTNode {
    explicit Type(ValueTypes raw_type, bool is_pointer = false, int number = 0);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    bool is_array();

    bool is_pointer{false};
    int number{0};
    ValueTypes raw_type;
};

struct Expression : public ASTNode {
};

struct BinaryExpression : public Expression {
    BinaryExpression(Expression* left, BinaryOperation operation, Expression* right);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryOperation operation;
};

struct UnaryExpression : public Expression {
    UnaryExpression(UnaryOperation op, Expression* expression);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::unique_ptr<Expression> expression;
    UnaryOperation operation;
};

struct PtrDereference : public Expression {
    explicit PtrDereference(Expression* value);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::unique_ptr<Expression> value;
};

struct Identifier : public Expression {
    explicit Identifier(std::string name);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::string name;
};

struct Statement : public ASTNode {

};


struct Block : public ASTNode {
public:
    void add_statement(Statement* statement);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

private:
    std::vector<std::unique_ptr<Statement>> statements;
};


struct Declaration : public Statement {
public:
    Declaration(Type* type, std::string name);

    Declaration(Type* type, std::string name, Expression* value);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::unique_ptr<Type> type;
    std::string name;
    std::unique_ptr<Expression> value;
};

struct Assignment : public Statement {
    Assignment(std::string name, Expression* value);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::string name;
    std::unique_ptr<Expression> value;
};

struct IFStatement : public Statement {
    IFStatement(Expression* condition, Block* affirmative, Block* negative = nullptr);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> affirmative;
    std::unique_ptr<Block> negative; // Nullable
};

struct WhileStatement : public Statement {
    WhileStatement(Expression* condition, Block* affirmative);

    std::string toString() override;

    void accept(ASTVisitor &v) override;

    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> affirmative;
};


std::string stringify(boost::any v, Type* type);

struct Literal : public Expression {
    Literal(boost::any value, Type* type);

    void accept(ASTVisitor &v) override;

    std::string toString() override;

    std::unique_ptr<Type> type;
    const boost::any value;
};


#endif //JOP_AST_H
