#include "If2SwitchVisitor.hpp"

#include "clang/AST/ParentMapContext.h"

/***********************
 * Shema transformacije
 * ---------------------
 * if (ako)
 *   onda;
 * else
 *   inace;
 * ---------------------
 * switch ((int)!!ako) {
 * case 1:
 *   onda;
 *   break;
 * default:
 *   inace;
 * }
 ***********************/

/* Provera da li je if pomocni */
bool If2SwitchVisitor::pomocni(IfStmt *s) const {
    /* Dohvatanje roditelja */
    const auto rod = TheASTContext.getParents(*s)
                     .begin()->get<CompoundStmt>();

    /* Odustajanje ako se ne uklapa */
    if (!rod) return false;

    /* Vektor dece roditelja */
    std::vector<const Stmt *> deca;
    for (const auto dete : rod->children())
        deca.push_back(dete);

    /* Odustajanje ako se ne uklapa */
    if (deca.size() != 3) return false;

    /* Dohvatanje konkretne dece */
    const auto dekl = isa<DeclStmt>(deca[0]);
    const auto if1 = isa<IfStmt>(deca[1]);
    const auto sw = isa<SwitchStmt>(deca[1]);
    const auto if2 = dyn_cast<IfStmt>(deca[2]);

    /* Odustajanje ako se ne uklapa */
    if (!dekl || (!if1 && !sw) || !if2) return false;

    /* Vracanje konacne provere */
    return s == if2;
}

/* Pretvaranje if naredbe u switch */
bool If2SwitchVisitor::VisitIfStmt(IfStmt *s) const {
    /* Odustajanje ako je pomocni */
    if (pomocni(s)) return true;

    /* Case klauza switcha */
    const auto cas = napraviCase(napraviInt(1), s->getThen());

    /* Iskakanje iz switcha */
    const auto br = napraviBreak();

    /* Default klauza switcha */
    const auto def = napraviDefault(s->getElse());

    /* Slozeno telo switcha */
    const auto telo = !s->getElse() ? napraviSlozenu({cas}) :
                                      napraviSlozenu({cas, br, def});

    /* Uslov kao istinitosna vrednost */
    const auto uslov = dohvatiIstinitost(s->getCond());

    /* Odgovarajuca switch naredba */
    const auto sw = napraviSwitch(uslov, telo);

    /* Tekstualna zamena koda */
    zameni(s, sw);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod if naredbe */
bool If2SwitchVisitor::TraverseIfStmt(IfStmt *s) {
    return WalkUpFromIfStmt(s);
}
