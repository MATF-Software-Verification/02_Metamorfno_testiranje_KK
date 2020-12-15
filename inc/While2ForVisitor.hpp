#ifndef WHILE2FOR_H
#define WHILE2FOR_H

#include "MTKConsumer.hpp"

/* Posetilac koji while pretvara u for */
class While2ForVisitor : public MTKVisitor<While2ForVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    While2ForVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Pretvaranje while petlji u for */
    bool VisitWhileStmt(WhileStmt *s) const;

    /* Prekid obilaska kod while petlje */
    bool TraverseWhileStmt(WhileStmt *s);
};

#endif
