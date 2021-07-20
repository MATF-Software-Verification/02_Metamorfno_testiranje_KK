#ifndef WHILE2GOTO_H
#define WHILE2GOTO_H

#include "MTKConsumer.hpp"
#include <unordered_map>
#include<iostream>
/* Posetilac koji while pretvara u goto */
class While2GotoVisitor : public MTKVisitor<While2GotoVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    While2GotoVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {
    }
    /* Pretvaranje while petlji u goto */
    bool VisitWhileStmt(WhileStmt *s) ;

    bool TraverseFunctionDecl(FunctionDecl*);
private:
    FunctionDecl* kontekstFunkcija_;
    std::string nazivKontekstFunkcije_;
};

#endif
