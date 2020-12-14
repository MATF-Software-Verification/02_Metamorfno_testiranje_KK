#ifndef PREPSWITCH_H
#define PREPSWITCH_H

#include "MTKConsumer.hpp"

#include <unordered_set>

/* Posetilac koji do pretvara u for */
class PrepSwitchVisitor : public MTKVisitor<PrepSwitchVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepSwitchVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Obrada odgovarajuceg switch */
    bool obradiSwitch(SwitchStmt *s);

    /* Priprema switch naredbe */
    bool VisitContinueStmt(ContinueStmt *s);

private:
    /* Privatno cuvanje vec pripremljenih naredbi */
    std::unordered_set<SwitchStmt *> prip;
};

#endif
