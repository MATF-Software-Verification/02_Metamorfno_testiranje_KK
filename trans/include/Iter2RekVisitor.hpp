#ifndef ITER2REK_H
#define ITER2REK_H

#include "MTKConsumer.hpp"

/* Posetilac koji iteraciju svodi na repnu rekurziju */
class Iter2RekVisitor : public MTKVisitor<Iter2RekVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    Iter2RekVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Provera ima li prepreka za izmenu */
    bool imaWhilePrepreka(Stmt *s) const;

    /* Provera ima li prepreka za izmenu */
    bool imaWhilePrepreka(WhileStmt *s) const;

    /* Dohvatanje deklaracija na koje se referise */
    void dohvatiDeklaracije(Stmt *s);

    /* Pravljenje rekurzivne funkcije */
    FunctionDecl *napraviRekFunkciju(WhileStmt *s) const;

    /* Eliminisanje svake (while) petlje */
    bool VisitWhileStmt(WhileStmt *s);

    /* Pamcenje tekuce funkcije */
    bool VisitFunctionDecl(FunctionDecl *f);

    /* Prolazak kroz sve (while) petlje */
    bool TraverseWhileStmt(WhileStmt *s);

private:
    /* Privatno cuvanje tekuce funkcije */
    FunctionDecl *tekf;

    /* Privatno cuvanje sadrzanih deklaracija */
    std::vector<VarDecl *> dekls;
    std::unordered_set<VarDecl *> deklm;
    std::unordered_set<VarDecl *> tabu;
};

#endif
