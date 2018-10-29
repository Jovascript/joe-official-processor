//
// Created by Joseph on 28/10/2018.
//

#ifndef JOP_ASTVISITOR_H
#define JOP_ASTVISITOR_H


#include "ast.h"

class ASTVisitor {
public:
    virtual void visit(Type* type) = 0;
    virtual void visit(Declaration* declaration) = 0;
    virtual void visit(Assignment* assignment) = 0;
    virtual void visit(BinaryExpression* binaryExpression) = 0;
    virtual void visit(UnaryExpression* unaryExpression) = 0;
    virtual void visit(PtrDereference* ptrDereference) = 0;
    virtual void visit(Identifier* identifier) = 0;
    virtual void visit(IFStatement* ifStatement) = 0;
    virtual void visit(WhileStatement* whileStatement) = 0;
    virtual void visit(Block* block) = 0;
    virtual void visit(Literal* literal) = 0;
};



#endif //JOP_ASTVISITOR_H
