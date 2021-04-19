#ifndef PREPSWITCH_H
#define PREPSWITCH_H

#include "MTKConsumer.hpp"

/* Posetilac koji priprema switch */
class PrepSwitchVisitor : public MTKVisitor<PrepSwitchVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepSwitchVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Provera ima li dubokih oznaka */
    bool proveriSwitchCase(SwitchCase *s) const;

    /* Obrada odgovarajuceg switcha */
    DeclRefExpr *obradiSwitch(const SwitchStmt *s);

    /* Priprema switch naredbe */
    bool VisitContinueStmt(ContinueStmt *s);

    /* Provera switch naredbe */
    bool VisitSwitchStmt(SwitchStmt *s);

    /* Nacin obrade deklaracije */
    bool TraverseDecl(Decl *d);

private:
    /* Privatno cuvanje tekuce deklaracije */
    Decl *tekdek;

    /* Tabu skup switcheva koji se ne mogu izmeniti */
    std::unordered_set<const SwitchStmt *> tabu;

    /* Privatno cuvanje vec pripremljenih naredbi */
    std::unordered_map<const SwitchStmt *, DeclRefExpr *> prip;
};

#endif
