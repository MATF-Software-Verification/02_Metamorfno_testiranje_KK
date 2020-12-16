#ifndef PREPFOR_H
#define PREPFOR_H

#include "MTKConsumer.hpp"

/* Posetilac koji dodaje korak petlje */
class PrepForVisitor : public MTKVisitor<PrepForVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepForVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Dodavanje inkrementa pre continue */
    bool VisitContinueStmt(ContinueStmt *s) const;
};

#endif
