#ifndef FOR2WHILE_H
#define FOR2WHILE_H

#include "LoopConsumer.hpp"

/* Posetilac koji for pretvara u while */
class For2WhileVisitor : public LoopVisitor<For2WhileVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    For2WhileVisitor(Rewriter &R, ASTContext &A)
      : LoopVisitor(R, A) {}

    /* Pretvaranje for petlji u while */
    bool VisitForStmt(ForStmt *s);

    /* Prekid obilaska kod for petlje */
    bool TraverseForStmt(ForStmt *s);
};

#endif
