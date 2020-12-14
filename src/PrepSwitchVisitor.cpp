#include "PrepSwitchVisitor.hpp"

/* Obrada odgovarajuceg switch */
bool PrepSwitchVisitor::obradiSwitch(SwitchStmt *s) {
    /* Odustajanje od pripremljenih */
    if (prip.count(s))
        return true;

    /* Nastavljanje dalje */
    return prip.insert(s).second;
}

/* Priprema switch naredbe */
bool PrepSwitchVisitor::VisitContinueStmt(ContinueStmt *s) {
    /* Nastavljanje dalje */
    return true;
}
