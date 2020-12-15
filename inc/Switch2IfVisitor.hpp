#ifndef SWITCH2IF_H
#define SWITCH2IF_H

#include "MTKConsumer.hpp"

/* Posetilac koji do pretvara u for */
class Switch2IfVisitor : public MTKVisitor<Switch2IfVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    Switch2IfVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Izracunavanje uslova za default */
    Expr *defUslov(StmtIterator dete,
                   StmtIterator kraj,
                   DeclRefExpr *uslov);

    /* Provera da li je prazan default */
    bool prazanDefault(DefaultStmt *s);

    /* Pretvaranje switch naredbe u if */
    bool VisitSwitchStmt(SwitchStmt *s);

    /* Prekid obilaska kod switch naredbe */
    bool TraverseSwitchStmt(SwitchStmt *s);

    /* Nacin obrade deklaracije */
    bool TraverseDecl(Decl *d);

private:
    /* Privatno cuvanje tekuce deklaracije */
    Decl *tekdek;
};

#endif
