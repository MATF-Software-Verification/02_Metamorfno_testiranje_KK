#include "While2ForVisitor.hpp"

/* Posetilac koji while pretvara u for */
bool While2ForVisitor::VisitWhileStmt(WhileStmt *s) {
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
