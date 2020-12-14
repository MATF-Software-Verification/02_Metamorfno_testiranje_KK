#ifndef PREPIF_H
#define PREPIF_H

#include "MTKConsumer.hpp"

#include <unordered_set>

/* Posetilac koji do pretvara u for */
class PrepIfVisitor : public MTKVisitor<PrepIfVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepIfVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Obrada odgovarajuceg if */
    bool obradiIf(IfStmt *s);

    /* Priprema if naredbe */
    bool VisitBreakStmt(BreakStmt *s);

private:
    /* Privatno cuvanje vec pripremljenih naredbi */
    std::unordered_set<IfStmt *> prip;
};

#endif
