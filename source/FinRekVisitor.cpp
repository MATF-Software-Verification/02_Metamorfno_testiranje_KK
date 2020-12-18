#include "FinRekVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 *
 * ---------------------
 *
 ***********************/

/* Zamena svakog iskakanja returnom */
bool FinRekVisitor::VisitBreakStmt(BreakStmt *s) const {
    return s;
}

/* Zamena svakog preskakanja returnom */
bool FinRekVisitor::VisitContinueStmt(ContinueStmt *s) const {
    return s;
}

/* Dereferenciranje svakog izraza iz petlje */
bool FinRekVisitor::VisitDeclRefExpr(DeclRefExpr *e) const {
    return e;
}

/* Obrada deklaracije funkcije */
bool FinRekVisitor::TraverseFunctionDecl(FunctionDecl *f) {
    /* Privatno cuvanje tekuce funkcije */
    tekf = f;

    /* Nastavljanje dalje ili povlacenje nazad */
    return !fje->count(f->getName().str()) ? WalkUpFromFunctionDecl(f) :
           RecursiveASTVisitor<FinRekVisitor>::TraverseFunctionDecl(f);
}
