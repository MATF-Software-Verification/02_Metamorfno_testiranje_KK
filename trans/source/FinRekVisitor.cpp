#include "FinRekVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * int petlja(int *x, int *y, int *ret) {
 *   if (x > 2) {
 *     y++;
 *     break;
 *     x = y;
 *     continue;
 *     int z;
 *     return 5;
 *     continue;
 *   }
 *   return 0;
 * }
 * ---------------------
 * int petlja(int *x, int *y, int *ret) {
 *   if (*x > 2) {
 *     (*y)++;
 *     return 0;
 *     *x = *y;
 *     return petlja(x, y);
 *     int z;
 *     {
 *       *ret = 5;
 *       return 1;
 *     }
 *     return petlja(x, y);
 *   }
 *   return 0;
 * }
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
bool FinRekVisitor::VisitDeclRefExpr(DeclRefExpr *s) const {
    return s;
}

/* Obrada deklaracije funkcije */
bool FinRekVisitor::TraverseFunctionDecl(FunctionDecl *f) {
    /* Privatno cuvanje tekuce funkcije */
    tekf = f;

    /* Nastavljanje dalje ili povlacenje nazad */
    return !fje->count(f->getName().str()) ? WalkUpFromFunctionDecl(f) :
           RecursiveASTVisitor<FinRekVisitor>::TraverseFunctionDecl(f);
}
