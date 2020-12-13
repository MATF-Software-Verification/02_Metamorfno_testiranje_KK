#include "For2WhileVisitor.hpp"
  
/* Posetilac koji for pretvara u while */
bool For2WhileVisitor::VisitForStmt(ForStmt *s) {
    /* Slozena naredba sa telom i inkrementacijom
     * ili samo telo ako nema inkrementacije */
    Stmt *telo;
    if (s->getInc()) {
      telo = napraviSlozenu({s->getBody(), s->getInc()});
    } else {
      telo = s->getBody();
    }

    /* While petlja sa novim telom i uslovom
     * ili beskonacna petlja ako nema uslova */
    auto *uslov = s->getCond();
    if (!uslov) {
      uslov = napraviTrue();
    }
    const auto petlja = napraviWhile(uslov, telo);

    /* Slozena naredba sa inicijalizacijom i petljom
     * ili samo petlja ako nema inicijalizacije */
    Stmt *initpet;
    if (s->getInit()) {
      initpet = napraviSlozenu({s->getInit(), petlja});
    } else {
      initpet = petlja;
    }

    /* Tekstualna zamena koda */
    zameni(s, initpet);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod while petlje */
bool For2WhileVisitor::TraverseForStmt(ForStmt *s) {
    return WalkUpFromForStmt(s);
}
