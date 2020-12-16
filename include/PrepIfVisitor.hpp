#ifndef PREPIF_H
#define PREPIF_H

#include "MTKConsumer.hpp"

#include <unordered_map>

/* Posetilac koji do pretvara u for */
class PrepIfVisitor : public MTKVisitor<PrepIfVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepIfVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Obrada odgovarajuceg if */
    DeclRefExpr *obradiIf(const IfStmt *s);

    /* Priprema if naredbe */
    bool VisitBreakStmt(BreakStmt *s);

    /* Nacin obrade deklaracije */
    bool TraverseDecl(Decl *d);

private:
    /* Privatno cuvanje tekuce deklaracije */
    Decl *tekdek;

    /* Privatno cuvanje vec pripremljenih naredbi */
    std::unordered_map<const IfStmt *, DeclRefExpr *> prip;
};

#endif
