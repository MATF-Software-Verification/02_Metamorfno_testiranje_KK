#include "PrepIfVisitor.hpp"

/* Obrada odgovarajuceg if */
bool PrepIfVisitor::obradiIf(IfStmt *s) {
    /* Odustajanje od pripremljenih */
    if (prip.count(s))
        return true;

    /* Nastavljanje dalje */
    return prip.insert(s).second;
}

/* Priprema if naredbe */
bool PrepIfVisitor::VisitBreakStmt(BreakStmt *s) {
    /* Nastavljanje dalje */
    return true;
}
