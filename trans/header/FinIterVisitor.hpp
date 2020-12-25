#ifndef FINITER_H
#define FINITER_H

#include "MTKConsumer.hpp"

/* Posetilac koji finalizira iteraciju */
class FinIterVisitor : public MTKVisitor<FinIterVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    FinIterVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Zamena svakog kriticnog return */
    bool VisitReturnStmt(ReturnStmt *s) const;

    /* Obrada deklaracije funkcije */
    bool TraverseFunctionDecl(FunctionDecl *f);

private:
    /* Privatno cuvanje tekuce funkcije */
    FunctionDecl *tekf;
};

#endif
