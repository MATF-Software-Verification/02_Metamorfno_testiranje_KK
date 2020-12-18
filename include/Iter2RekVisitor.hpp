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

    /* Dohvatanje deklaracija na koje se referise */
    void dohvatiDeklaracije(Stmt *s);

    /* Eliminisanje svake (while) petlje */
    bool VisitWhileStmt(WhileStmt *s);

    /* Prolazak kroz sve (while) petlje */
    bool TraverseWhileStmt(WhileStmt *s);

private:
    /* Privatno cuvanje sadrzanih deklaracija */
    std::unordered_set<VarDecl *> dekls;
    std::unordered_set<VarDecl *> tabu;
};

#endif
