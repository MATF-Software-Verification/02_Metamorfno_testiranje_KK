#ifndef FOR2WHILE_H
#define FOR2WHILE_H

#include "MTKConsumer.hpp"

/* Posetilac koji for pretvara u while */
class For2WhileVisitor : public MTKVisitor<For2WhileVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    For2WhileVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Pretvaranje for petlji u while */
    bool VisitForStmt(ForStmt *s);

    /* Prekid obilaska kod for petlje */
    bool TraverseForStmt(ForStmt *s);
};

#endif
