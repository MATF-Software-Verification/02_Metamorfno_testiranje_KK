#ifndef FINREK_H
#define FINREK_H

#include "MTKConsumer.hpp"

/* Posetilac koji finalizira rekurziju */
class FinRekVisitor : public MTKVisitor<FinRekVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    FinRekVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Zamena svakog iskakanja returnom */
    bool VisitBreakStmt(BreakStmt *s) const;

    /* Zamena svakog preskakanja returnom */
    bool VisitContinueStmt(ContinueStmt *s) const;

    /* Dereferenciranje svakog izraza iz petlje */
    bool VisitDeclRefExpr(DeclRefExpr *s) const;

    /* Obrada deklaracije funkcije */
    bool TraverseFunctionDecl(FunctionDecl *f);

private:
    /* Privatno cuvanje tekuce funkcije */
    FunctionDecl *tekf;
};

#endif
