#include "FinIterVisitor.hpp"

/******************************
 * Shema transformacije
 * ----------------------------
 * fja(a, ..., b) {
 *   while (!uslov) {
 *     telo2;
 *     return rek(a0, ..., b0);
 *   }
 *   telo1;
 *   return baza;
 * }
 * ----------------------------
 * fja(a, ..., b) {
 *   while (!uslov) {
 *     telo2;
 *     a0 = ...; b0 = ...;
 *     a = a0; b = b0;
 *   }
 *   telo1;
 *   return baza;
 * }
 ******************************/

/* Zamena svakog kriticnog return */
bool FinIterVisitor::VisitReturnStmt(ReturnStmt *s) const {
    /* Dohvatanje poziva iz povratne vrednosti */
    const auto call = dyn_cast_or_null<CallExpr>(s->getRetValue());

    /* Odustajanje ako nije tekuca funkcija */
    if (!call || call->getDirectCallee() != tekf) return true;

    /* Nastavljanje dalje */
    return true;
}

/* Obrada deklaracije funkcije */
bool FinIterVisitor::TraverseFunctionDecl(FunctionDecl *f) {
    /* Privatno cuvanje tekuce funkcije */
    tekf = f;

    /* Nastavljanje dalje ili povlacenje nazad */
    return !fje->count(f->getName().str()) ? WalkUpFromFunctionDecl(f) :
           RecursiveASTVisitor<FinIterVisitor>::TraverseFunctionDecl(f);
}
