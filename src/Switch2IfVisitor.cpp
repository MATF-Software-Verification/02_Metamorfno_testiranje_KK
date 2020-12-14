#include "Switch2IfVisitor.hpp"

/* Pretvaranje switch naredbe u if */
bool Switch2IfVisitor::VisitSwitchStmt(SwitchStmt *s) {
    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod switch naredbe */
bool Switch2IfVisitor::TraverseSwitchStmt(SwitchStmt *s) {
    return WalkUpFromSwitchStmt(s);
}
