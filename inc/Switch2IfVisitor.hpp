#ifndef SWITCH2IF_H
#define SWITCH2IF_H

#include "MTKConsumer.hpp"

/* Posetilac koji do pretvara u for */
class Switch2IfVisitor : public MTKVisitor<Switch2IfVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    Switch2IfVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Pretvaranje switch naredbe u if */
    bool VisitSwitchStmt(SwitchStmt *s);

    /* Prekid obilaska kod switch naredbe */
    bool TraverseSwitchStmt(SwitchStmt *s);
};

#endif
