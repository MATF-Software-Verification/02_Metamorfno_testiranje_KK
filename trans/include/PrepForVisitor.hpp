#ifndef PREPFOR_H
#define PREPFOR_H

#include "MTKConsumer.hpp"

#include <stack>

/* Posetilac koji dodaje korak petlje */
class PrepForVisitor : public MTKVisitor<PrepForVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepForVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Virtuelni dekstruktor za brojanje prolaza */
    virtual ~PrepForVisitor();

    /* Dodavanje inkrementa pre continue */
    bool VisitContinueStmt(ContinueStmt *s) const;

    /* Dohvatanje deklaracija */
    void dohvatiDeklaracije(Stmt *s);

    /* Odmaskiranje deklaracija */
    void odmaskirajDeklaracije(Stmt *s);

    /* Odmaskiranje tela svake for petlje */
    bool VisitForStmt(ForStmt* s);

    /* Pamcenje tekuce while petlje */
    bool TraverseWhileStmt(WhileStmt *s);

    /* Pamcenje tekuce do petlje */
    bool TraverseDoStmt(DoStmt *s);

    /* Obilazak for petlje u zavisnosti od faze */
    bool TraverseForStmt(ForStmt *s);

    /* Podatak o tome da li je bilo vise prolaza */
    static bool imaloPosla();

private:
    /* Privatno cuvanje steka petlji */
    std::stack<Stmt *> petlje;

    /* Privatno cuvanje sadrzanih deklaracija */
    std::unordered_set<std::string> dekls;

    /* Staticki podatak da li je prvi prolaz */
    static bool prviProlaz;
};

#endif
