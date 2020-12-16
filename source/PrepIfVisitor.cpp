#include "PrepIfVisitor.hpp"

#include "clang/AST/ParentMapContext.h"

/* Shema transformacije
 * ---------------------
 * if (ako) {
 *   onda1;
 *   break;
 *   onda2;
 * } else {
 *   inace1;
 *   break;
 *   inace2;
 * }
 * ---------------------
 * int br = 0;
 * if (ako) {
 *   onda1;
 *   {
 *     br = 1;
 *     break;
 *   }
 *   onda2;
 * } else {
 *   inace1;
 *   {
 *     br = 1;
 *     break;
 *   }
 *   inace2;
 * }
 * if (br)
 *   break;
 * */

/* Obrada odgovarajuceg if */
DeclRefExpr *PrepIfVisitor::obradiIf(const IfStmt *s) {
    /* Odustajanje od pripremljenih */
    if (prip.count(s))
        return prip[s];

    /* Deklaracija uslovne promenljive */
    const auto dekl = napraviUslovnu(tekdek, "br", false);

    /* Obmotavanje ifa deklaracijom */
    dodajIspred(s, dekl);

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl);

    /* Break za skok iz petlje ili switcha */
    const auto br = napraviBreak();

    /* Uslov iskakanja iz petlje ili switcha */
    const auto skok = napraviIf(uslov, br);

    /* Obmotavanje ifa skokom */
    dodajIza(s, skok);

    /* Nastavljanje dalje */
    return prip[s] = uslov;
}

/* Priprema if naredbe */
bool PrepIfVisitor::VisitBreakStmt(BreakStmt *s) {
    /* Inicijalizacija deklaracije */
    DeclRefExpr *dekl = nullptr;

    /* Indikator posebne zamene ukoliko je
     * break direktni potomak else grane */
    bool dir = false;

    /* Prolazak kroz roditelje tekuceg break */
    auto rod = TheASTContext.getParentMapContext().getParents(*s);
    while (!rod.empty()) {
        /* Izdvajanje glavnog roditelja */
        const auto r = rod.begin()->get<Stmt>();

        /* Odustajanje ako je neka petlja ili switch */
        if (isa<DoStmt>(r)  || isa<WhileStmt>(r) ||
            isa<ForStmt>(r) || isa<SwitchStmt>(r))
            return true;

        /* Uzimanje roditelja koji je if */
        if (const auto rr = dyn_cast<IfStmt>(r)) {
            dekl = obradiIf(rr);
            dir = s == rr->getElse();
            break;
        }

        /* Nastavljanje dalje */
        rod = TheASTContext.getParents(*r);
    }

    /* Odustajanje ako nije switch roditelj */
    if (!dekl) return true;

    /* Postavljanje zastavice za skok iz petlje */
    const auto dodela = napraviDodelu(dekl, napraviTrue());

    /* Break za skok iz switcha */
    const auto iskoci = napraviBreak();

    /* Pravljenje nove naredbe */
    const auto zamena = napraviSlozenu({dodela, iskoci});

    /* Tekstualna zamena koda */
    zameni(s, zamena, dir);

    /* Nastavljanje dalje */
    return true;
}

/* Nacin obrade deklaracije */
bool PrepIfVisitor::TraverseDecl(Decl *d) {
    /* Privatno cuvanje tekuce deklaracije */
    tekdek = d;

    /* Nastavljanje dalje */
    return RecursiveASTVisitor<PrepIfVisitor>::TraverseDecl(d);
}
