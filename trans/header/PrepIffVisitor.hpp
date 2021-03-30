#ifndef PREPIFF_H
#define PREPIFF_H

#include "MTKConsumer.hpp"

/* Posetilac koji priprema if */
class PrepIffVisitor : public MTKVisitor<PrepIffVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepIffVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Provera da li je slozeno delo */
    bool slozenoTelo(IfStmt *s);

    /* Dorada tela if naredbe */
    bool VisitIfStmt(IfStmt *s);

    /* Prekid obilaska kod prvog if */
    bool TraverseIfStmt(IfStmt *s);
};

#endif
