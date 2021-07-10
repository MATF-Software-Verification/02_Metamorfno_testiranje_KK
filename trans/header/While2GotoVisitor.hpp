#ifndef WHILE2FOR_H
#define WHILE2FOR_H

#include "MTKConsumer.hpp"

/* Posetilac koji while pretvara u for */
class While2GotoVisitor : public MTKVisitor<While2GotoVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    While2GotoVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Pretvaranje while petlji u for */
    bool VisitWhileStmt(WhileStmt *s) ;

    /* Prekid obilaska kod while petlje */
    bool TraverseWhileStmt(WhileStmt *s);

    bool TraverseFunctionDecl(FunctionDecl*);
private:
    FunctionDecl *kontekstFunkcijaDecl_;
    size_t jedinstveniIdLabelePetlji_{0};
};

#endif
