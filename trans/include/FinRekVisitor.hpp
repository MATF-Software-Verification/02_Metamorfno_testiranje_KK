#ifndef FINREK_H
#define FINREK_H

#include "MTKConsumer.hpp"

/* Posetilac koji finalizira rekurziju */
class FinRekVisitor : public MTKVisitor<FinRekVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    FinRekVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Zamena svakog iskakanja returnom */
    bool VisitBreakStmt(BreakStmt *s) const;

    /* Zamena svakog preskakanja returnom */
    bool VisitContinueStmt(ContinueStmt *s) const;

    /* Zamena svakog return slozenom verzijom */
    bool VisitReturnStmt(ReturnStmt *s) const;

    /* Obrada izraza na odgovarajuci nacin */
    ParenExpr *obradiIzraz(DeclRefExpr *s);

    /* Hvatanje izraza iz unarnog operatora */
    bool VisitUnaryOperator(UnaryOperator *s);

    /* Dereferenciranje svakog izraza iz petlje */
    bool VisitDeclRefExpr(DeclRefExpr *s);

    /* Zamena gotovog for novom if naredbom */
    bool VisitForStmt(ForStmt *s);

    /* Obrada svake (for) petlje ili ne */
    bool TraverseForStmt(ForStmt *s);

    /* Obrada deklaracije funkcije */
    bool TraverseFunctionDecl(FunctionDecl *f);

private:
    /* Privatno cuvanje tekuce funkcije */
    FunctionDecl *tekf;

    /* Privatno cuvanje posecenih izraza */
    std::unordered_set<DeclRefExpr *> izr;
};

#endif
