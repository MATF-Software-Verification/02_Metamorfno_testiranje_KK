#ifndef IF2SWITCH_H
#define IF2SWITCH_H

#include "MTKConsumer.hpp"

/* Posetilac koji if pretvara u switch */
class If2SwitchVisitor : public MTKVisitor<If2SwitchVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    If2SwitchVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Provera da li je if pomocni */
    bool pomocni(IfStmt *s) const;

    /* Provera da li je unutrasnji if */
    bool imaIf(const Stmt *s) const;

    /* Pretvaranje if naredbe u switch */
    bool VisitIfStmt(IfStmt *s) const;
};

#endif
