#include "PrepSwitchVisitor.hpp"

/***********************
 * Shema transformacije
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
 ***********************/

/* Provera ima li dubokih oznaka */
bool PrepSwitchVisitor::proveriSwitchCase(SwitchCase *s) const {
    /* Dohvatanje prvog roditelja */
    const auto telo = dyn_cast<CompoundStmt>(rods.at(s));
    if (!telo) return false;

    /* Dohvatanje drugog roditelja */
    const auto swch = dyn_cast<SwitchStmt>(rods.at(telo));
    if (!swch) return false;

    /* Sve je u redu u ovom trenutku */
    return true;
}

/* Obrada odgovarajuceg switcha */
DeclRefExpr *PrepSwitchVisitor::obradiSwitch(const SwitchStmt *s) {
    /* Odustajanje od tabu switcheva */
    if (tabu.count(s)) return nullptr;

    /* Odustajanje od pripremljenih */
    if (prip.count(s)) return prip[s];

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
    auto r = rods.at(s);
    while (r) {
        /* Odustajanje ako je neka petlja */
        if (isa<DoStmt>(r) || isa<WhileStmt>(r) || isa<ForStmt>(r))
            return true;

        /* Uzimanje roditelja koji je switch */
        if (const auto rr = dyn_cast<SwitchStmt>(r)) {
            dekl = obradiSwitch(rr);
            break;
        }

        /* Nastavljanje dalje */
        r = rods.at(r);
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

/* Provera switch naredbe */
bool PrepSwitchVisitor::VisitSwitchStmt(SwitchStmt *s) {
    /* Prolazak kroz svaki switch case */
    for (auto swc = s->getSwitchCaseList();
         swc; swc = swc->getNextSwitchCase())
        /* Odustajanje ako postoje duboki */
        if (!proveriSwitchCase(swc)) {
            tabu.insert(s);
            return true;
        }

    /* Nastavljanje dalje */
    return true;
}

/* Nacin obrade deklaracije */
bool PrepSwitchVisitor::TraverseDecl(Decl *d) {
    /* Privatno cuvanje tekuce deklaracije */
    tekdek = d;

    /* Nastavljanje dalje */
    return MTKVisitor::TraverseDecl(d);
}
