#include "While2ForVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * while (cond)
 *   telo;
 * ---------------------
 * for (; cond;)
 *   telo;
 ***********************/

/* Posetilac koji while pretvara u for */
bool While2ForVisitor::VisitWhileStmt(WhileStmt *s) const {
    /* Formiranje nove for petlje */
    const auto petlja = napraviFor(s->getCond(), nullptr, s->getBody());

    /* Tekstualna zamena koda */
    zameni(s, petlja);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod while petlje */
bool While2ForVisitor::TraverseWhileStmt(WhileStmt *s) {
    return WalkUpFromWhileStmt(s);
}
