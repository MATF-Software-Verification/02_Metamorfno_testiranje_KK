#ifndef WHILE2DO_H
#define WHILE2DO_H

#include "MTKConsumer.hpp"

/* Posetilac koji while pretvara u do */
class While2DoVisitor : public MTKVisitor<While2DoVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    While2DoVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Pretvaranje while petlji u do-while */
    bool VisitWhileStmt(WhileStmt *s);

    /* Prekid obilaska kod while petlje */
    bool TraverseWhileStmt(WhileStmt *s);
};

#endif
