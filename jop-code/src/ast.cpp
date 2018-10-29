#include <utility>

//
// Created by Joseph on 21/10/2018.
//

#include "ast.h"
#include "computils.h"

#include "ASTVisitor.h"


Declaration::Declaration(Type *type, std::string name): type(type), name(std::move(name)) {

}

Declaration::Declaration(Type *type, std::string name, Expression *value): type(type), name(std::move(name)), value(value) {

}

std::string Declaration::toString() {
    std::string res = "Declaration(" + name + ")\n";
    res += indent(type->toString());
    if (value) {
        res += indent(value->toString());
    }
    return res;
}



void Block::add_statement(Statement *statement) {
    statements.push_back(std::unique_ptr<Statement>(statement));
}

std::string Block::toString() {
    std::string res = "Block()\n";
    for (auto& statement: statements) {
        res += indent(statement->toString());
    }
    return res;
}



Type::Type(ValueTypes raw_type, bool is_pointer, int number): is_pointer(is_pointer), number(number), raw_type(raw_type) {

}

std::string Type::toString() {
    std::string s = "Type(";
    s += type_names[raw_type];
    if (number == 0) {
        return is_pointer ? s + "*)\n" : s + ")\n";
    } else {
        return number == -1 ? s + "[])\n" : s + "[" + std::to_string(number) + "])\n";
    }
}



PtrDereference::PtrDereference(Expression* value) : value(value) {}

std::string PtrDereference::toString() {
    std::string res = "PtrDeref()\n";
    res += indent(value->toString());
    return res;
}



Identifier::Identifier(std::string name): name(std::move(name)) {}

std::string Identifier::toString() {
    return std::string("Identifier(") + name + ")\n";
}


Assignment::Assignment(std::string name, Expression *value): name(std::move(name)), value(value) {

}

std::string Assignment::toString() {
    std::string res = "Declaration(" + name + ")\n";
        res += indent(value->toString());
    return res;
}



IFStatement::IFStatement(Expression *condition, Block *affirmative, Block *negative): condition(condition), affirmative(affirmative), negative(negative) {
}

std::string IFStatement::toString() {
    std::string res = "IFStatement()\n";
    res += indent(condition->toString());
    res += indent(affirmative->toString());
    if (negative) {
        res += indent(negative->toString());
    }
    return res;
}



WhileStatement::WhileStatement(Expression *condition, Block *affirmative): condition(condition), affirmative(affirmative) {

}

std::string WhileStatement::toString() {
    std::string res = "WhileStatement()\n";
    res += indent(condition->toString());
    res += indent(affirmative->toString());
    return res;
}



BinaryExpression::BinaryExpression(Expression *left, BinaryOperation operation, Expression *right): left(left), operation(operation), right(right) {

}

std::string BinaryExpression::toString() {
    std::string res = std::string("BinaryExpression(") + binop_symbols[operation] + ")\n";
    res += indent(left->toString());
    res += indent(right->toString());
    return res;
}

UnaryExpression::UnaryExpression(UnaryOperation op, Expression *expression): operation(op), expression(expression) {

}

std::string UnaryExpression::toString() {
    std::string res = std::string("UnaryOperation(") + unop_symbols[operation] + ")\n";
    res += indent(expression->toString());
    return res;
}

std::string stringify(boost::any v, Type* type) {
    if (type->is_array()) {
        switch (type->raw_type) {
            case CHAR: return stringify(boost::any_cast<std::vector<char>>(v));
            case UINT: return stringify(boost::any_cast<std::vector<unsigned int>>(v));
            case INT: return stringify(boost::any_cast<std::vector<int>>(v));
            case BOOL: return stringify(boost::any_cast<std::vector<bool>>(v));
        }
    } else {
        switch (type->raw_type) {
            case CHAR: return stringify(boost::any_cast<char>(v));
            case UINT: return stringify(boost::any_cast<unsigned int>(v));
            case INT: return stringify(boost::any_cast<int>(v));
            case BOOL: return stringify(boost::any_cast<bool>(v));
        }
    }
}

Literal::Literal(boost::any value, Type* type): value(std::move(value)), type(type) {}

std::string Literal::toString() {
    return "Literal(" + stringify(value, type.get()) + ")\n";
}

void Assignment::accept(ASTVisitor &v) {
    v.visit(this);
}

void Declaration::accept(ASTVisitor &v) {
    v.visit(this);
}
void Block::accept(ASTVisitor &v) {
    v.visit(this);
}

void PtrDereference::accept(ASTVisitor &v) {
    v.visit(this);

}
void Type::accept(ASTVisitor &v) {
    v.visit(this);

}

bool Type::is_array() {
    return number != 0;
}

void Identifier::accept(ASTVisitor &v) {
    v.visit(this);
};

void BinaryExpression::accept(ASTVisitor &v) {
    v.visit(this);
}

void UnaryExpression::accept(ASTVisitor &v) {
    v.visit(this);
}

void IFStatement::accept(ASTVisitor &v) {
    v.visit(this);
}

void WhileStatement::accept(ASTVisitor &v) {
    v.visit(this);

}

void Literal::accept(ASTVisitor &v) {
    v.visit(this);
}


