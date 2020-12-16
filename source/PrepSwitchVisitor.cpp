#include "PrepSwitchVisitor.hpp"

#include "clang/AST/ParentMapContext.h"

/* Shema transformacije
 * ---------------------
 * switch (uslov) {
 *   case a:
 *     teloa1;
 *     continue;
 *     teloa2;
 *
 *   default:
 *     telod1;
 *     continue;
 *     telod2;
 * }
 * ---------------------
 * int cont = 0;
 * switch (uslov) {
 *   case a:
 *     teloa1;
 *     {
 *       cont = 1;
 *       continue;
 *     }
 *     teloa2;
 *
 *   default:
 *     telod1;
 *     {
 *       cont = 1;
 *       continue;
 *     }
 *     telod2;
 * }
 * if (cont)
 *   continue;
 * */

/* Obrada odgovarajuceg switch */
DeclRefExpr *PrepSwitchVisitor::obradiSwitch(const SwitchStmt *s) {
    /* Odustajanje od pripremljenih */
    if (prip.count(s))
        return prip[s];

    /* Deklaracija uslovne promenljive */
    const auto dekl = napraviUslovnu(tekdek, "cont", false);

    /* Obmotavanje switcha deklaracijom */
    dodajIspred(s, dekl);

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl);

    /* Continue za skok iz petlje */
    const auto cont = napraviCont();

    /* Uslov iskakanja iz petlje */
    const auto skok = napraviIf(uslov, cont);

    /* Obmotavanje switcha skokom */
    dodajIza(s, skok);

    /* Nastavljanje dalje */
    return prip[s] = uslov;
}

/* Priprema switch naredbe */
bool PrepSwitchVisitor::VisitContinueStmt(ContinueStmt *s) {
    /* Inicijalizacija deklaracije */
    DeclRefExpr *dekl = nullptr;

    /* Prolazak kroz roditelje tekuceg continue */
    auto rod = TheASTContext.getParentMapContext().getParents(*s);
    while (!rod.empty()) {
        /* Izdvajanje glavnog roditelja */
        const auto r = rod.begin()->get<Stmt>();

        /* Odustajanje ako je neka petlja */
        if (isa<DoStmt>(r) || isa<WhileStmt>(r) || isa<ForStmt>(r))
            return true;

        /* Uzimanje roditelja koji je switch */
        if (const auto rr = dyn_cast<SwitchStmt>(r)) {
            dekl = obradiSwitch(rr);
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
    zameni(s, zamena);

    /* Nastavljanje dalje */
    return true;
}

/* Nacin obrade deklaracije */
bool PrepSwitchVisitor::TraverseDecl(Decl *d) {
    /* Privatno cuvanje tekuce deklaracije */
    tekdek = d;

    /* Nastavljanje dalje */
    return RecursiveASTVisitor<PrepSwitchVisitor>::TraverseDecl(d);
}
