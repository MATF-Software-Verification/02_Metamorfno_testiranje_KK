#include "While2DoVisitor.hpp"

/* Posetilac koji while pretvara u do */
bool While2DoVisitor::VisitWhileStmt(WhileStmt *s) {
    /* Odgovarajuca do-while verzija */
    const auto petlja = napraviDo(s->getBody(), s->getCond());

    /* If iskaz za proveru uslova petlje */
    const auto uslov = napraviIf(s->getCond(), petlja);

    /* Tekstualna zamena koda */
    zameni(s, uslov);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod while petlje */
bool While2DoVisitor::TraverseWhileStmt(WhileStmt *s) {
    return WalkUpFromWhileStmt(s);
}
