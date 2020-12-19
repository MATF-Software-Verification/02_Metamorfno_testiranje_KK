#ifndef FOR2DO_H
#define FOR2DO_H

#include "MTKConsumer.hpp"

/* Posetilac koji for pretvara u do */
class For2DoVisitor : public MTKVisitor<For2DoVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    For2DoVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Pretvaranje for petlji u do-while */
    bool VisitForStmt(ForStmt *s) const;

    /* Prekid obilaska kod for petlje */
    bool TraverseForStmt(ForStmt *s);
};

#endif
