#ifndef LOOPUNROLL_H
#define LOOPUNROLL_H

#include "MTKConsumer.hpp"

/* Posetilac koji odmotava petlje */
class LoopUnrollVisitor : public MTKVisitor<LoopUnrollVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    LoopUnrollVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Postavljanje broja odmotavanja */
    static void postaviBroj(unsigned long long i);

    /* Umnozavanje tela do petlje */
    void umnoziTelo(DoStmt *s) const;

    /* Umnozavanje tela while petlje */
    void umnoziTelo(WhileStmt *s) const;

    /* Umnozavanje tela for petlje */
    void umnoziTelo(ForStmt *s) const;

    /* Obrada dece svake najvise petlje */
    void obradiDecu(Stmt *s) const;

    /* Provera ima li prepreka za izmenu */
    bool imaLabelStmt(Stmt *s) const;

    /* Poseta svake petlje na isti nacin */
    bool posetiPetlju(Stmt *s) const;

    /* Umnozavanje tela svake do petlje */
    bool VisitDoStmt(DoStmt *s) const;

    /* Umnozavanje tela svake while petlje */
    bool VisitWhileStmt(WhileStmt *s) const;

    /* Umnozavanje tela svake for petlje */
    bool VisitForStmt(ForStmt *s) const;

    /* Prekid obilaska kod prve do petlje */
    bool TraverseDoStmt(DoStmt *s);

    /* Prekid obilaska kod prve while petlje */
    bool TraverseWhileStmt(WhileStmt *s);

    /* Prekid obilaska kod prve for petlje */
    bool TraverseForStmt(ForStmt *s);

private:
    /* Staticki podatak o broju odmotavanja */
    static unsigned long long n;
};

#endif
