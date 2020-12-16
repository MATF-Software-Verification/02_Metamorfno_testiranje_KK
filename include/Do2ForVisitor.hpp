#ifndef DO2FOR_H
#define DO2FOR_H

#include "MTKConsumer.hpp"

/* Posetilac koji do pretvara u for */
class Do2ForVisitor : public MTKVisitor<Do2ForVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    Do2ForVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Pretvaranje do-while petlji u for */
    bool VisitDoStmt(DoStmt *s) const;

    /* Prekid obilaska kod do petlje */
    bool TraverseDoStmt(DoStmt *s);

    /* Nacin obrade deklaracije */
    bool TraverseDecl(Decl *d);

private:
    /* Privatno cuvanje tekuce deklaracije */
    Decl *tekdek;
};

#endif
