#ifndef DO2FOR_H
#define DO2FOR_H

#include "LoopConsumer.hpp"

/* Posetilac koji do pretvara u for */
class Do2ForVisitor : public LoopVisitor<Do2ForVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    Do2ForVisitor(Rewriter &R, ASTContext &A)
      : LoopVisitor(R, A) {}

    /* Pretvaranje do-while petlji u for */
    bool VisitDoStmt(DoStmt *s);

    /* Prekid obilaska kod do petlje */
    bool TraverseDoStmt(DoStmt *s);

    /* Nacin obrade deklaracije */
    bool TraverseDecl(Decl *d);

private:
    /* Privatno cuvanje tekuce deklaracije */
    Decl *tekdek;
};

#endif
