#include "If2SwitchVisitor.hpp"

/* Pretvaranje if naredbe u switch */
bool If2SwitchVisitor::VisitIfStmt(IfStmt *s) {
    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod if naredbe */
bool If2SwitchVisitor::TraverseIfStmt(IfStmt *s) {
    return WalkUpFromIfStmt(s);
}
