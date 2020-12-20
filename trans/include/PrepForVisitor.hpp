#ifndef PREPFOR_H
#define PREPFOR_H

#include "MTKConsumer.hpp"

#include <unordered_map>

/* Posetilac koji dodaje korak petlje */
class PrepForVisitor : public MTKVisitor<PrepForVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    PrepForVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Odredjivanje petlje i prethodnog */
    std::pair<const ForStmt *,
              const Stmt *> odrediPetlju(Stmt *s) const;

    /* Pronalazak svih imena u koraku */
    bool VisitDeclRefExpr(DeclRefExpr *s);

    /* Pronalazak svih imena u telu petlje */
    bool VisitDeclStmt(DeclStmt *s);

    /* Dodavanje inkrementa pre continue */
    bool VisitContinueStmt(ContinueStmt *s) const;

private:
    /* Privatno cuvanje mape imena */
    std::unordered_map<const ForStmt *,
                       std::unordered_set<std::string>> imena;

    /* Privatno cuvanje skupa maskiranih */
    std::unordered_set<const ForStmt *> maskirani;

    /* Poruka da maskiranje koraka pravi problem */
    static constexpr auto maskiranje = "Telo petlje maskira korak!";
};

#endif
