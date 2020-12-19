#include "For2DoVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * for (init; cond; inc)
 *   telo;
 * ---------------------
 * init;
 * if (cond)
 *   do {
 *     telo;
 *     inc;
 * } while (cond);
 ***********************/

/* Posetilac koji for pretvara u do */
bool For2DoVisitor::VisitForStmt(ForStmt *s) const {
    /* Slozena naredba sa telom i inkrementacijom
     * ili samo telo ako nema inkrementacije */
    Stmt *telo;
    if (s->getInc()) {
      telo = napraviSlozenu({s->getBody(), s->getInc()});
    } else {
      telo = s->getBody();
    }

    /* Do petlja sa novim telom i uslovom
     * ili beskonacna petlja ako nema uslova */
    auto *cond = s->getCond();
    if (!cond) {
      cond = napraviTrue();
    }
    const auto petlja = napraviDo(telo, cond);

    /* If naredba za proveru uslova petlje */
    const auto uslov = napraviIf(cond, petlja);

    /* Slozena naredba sa inicijalizacijom i petljom
     * ili samo petlja ako nema inicijalizacije */
    Stmt *initpet;
    if (s->getInit()) {
      initpet = napraviSlozenu({s->getInit(), uslov});
    } else {
      initpet = uslov;
    }

    /* Tekstualna zamena koda */
    zameni(s, initpet);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod for petlje */
bool For2DoVisitor::TraverseForStmt(ForStmt *s) {
    return WalkUpFromForStmt(s);
}
