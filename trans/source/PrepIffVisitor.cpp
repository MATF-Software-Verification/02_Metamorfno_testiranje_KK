#include "PrepIffVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * if (ako)
 *   onda;
 * else
 *   inace;
 * ---------------------
 * if (ako) {
 *   onda;
 * } else {
 *   inace;
 * }
 ***********************/

/* Provera da li je slozeno delo */
bool PrepIffVisitor::slozenoTelo(IfStmt *s) {
    return isa<CompoundStmt>(s->getThen()) &&
          (!s->getElse() || isa<CompoundStmt>(s->getElse()));
}

/* Dorada tela if naredbe */
bool PrepIffVisitor::VisitIfStmt(IfStmt *s) {
    /* Dopuna then grane ako treba */
    const auto then = s->getThen();
    if (!isa<CompoundStmt>(then)) {
        dodajIspred(then); dodajIza(then);
    }

    /* Dopuna else grane ako treba */
    const auto elses = s->getElse();
    if (elses && !isa<CompoundStmt>(elses)) {
        dodajIspred(elses); dodajIza(elses);
    }

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod prvog if */
bool PrepIffVisitor::TraverseIfStmt(IfStmt *s) {
    return !slozenoTelo(s) ? WalkUpFromIfStmt(s) :
           RecursiveASTVisitor<PrepIffVisitor>::TraverseIfStmt(s);
}
