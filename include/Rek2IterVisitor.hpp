#ifndef REK2ITER_H
#define REK2ITER_H

#include "MTKConsumer.hpp"

/* Posetilac koji repnu rekurziju svodi na iteraciju */
class Rek2IterVisitor : public MTKVisitor<Rek2IterVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    Rek2IterVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Enumeracija return statusa */
    enum class Status {
        NIJE_RETURN, REK_RETURN, NEREK_RETURN
    };

    /* Odredjivanje return statusa */
    Status retStatus(Stmt *s) const;

    /* Provera da li je baza rekurzivna */
    bool rekurzivnaBaza(Stmt *s) const;

    /* Pravljenje odgovarajuce zamene */
    CompoundStmt *napraviZamenu(Expr *uslov, Stmt *iter,
                                Stmt *baza, bool neg) const;

    /* Obrada nerek-then i rek-else */
    Stmt *obradiITENR() const;

    /* Obrada rek-then i nerek-else */
    Stmt *obradiITERN() const;

    /* Obrada slucaja if-then-else */
    Stmt *obradiIfThenElse() const;

    /* Sakupljanje dece iz opsega */
    Stmt *sakupiDecu(StmtIterator start, StmtIterator end) const;

    /* Obrada nerek-then i rek-tela */
    Stmt *obradiITTNR() const;

    /* Obrada rek-then i nerek-tela */
    Stmt *obradiITTRN() const;

    /* Obrada slucaja if-then-telo */
    Stmt *obradiIfThenTelo() const;

    /* Obrada deklaracije funkcije */
    bool VisitFunctionDecl(FunctionDecl *f);

private:
    /* Privatno cuvanje tekuce funkcije */
    FunctionDecl *tekf;
    CompoundStmt *telo;
    IfStmt *iff;
};

#endif
