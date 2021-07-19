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

    bool VisitBreakStmt(BreakStmt* s);

    bool VisitContinueStmt(ContinueStmt *s);

    bool TraverseFunctionDecl(FunctionDecl*);
private:
    size_t id{0};

    FunctionDecl *kontekstFunkcijaDecl_;
    static size_t jedinstveniIdLabelePetlji_;
    struct WhileLoopLabels {
        std::string whileLoopPocetakLabelaNaziv;
        std::string whileLoopKrajLabelaNaziv;
    };
    WhileStmt* trenutiWhile{nullptr};
    static std::unordered_map<size_t, WhileLoopLabels> whileLoopGotoLabels;
    static std::unordered_set<const Stmt*> netrebaZamena;
};

#endif
