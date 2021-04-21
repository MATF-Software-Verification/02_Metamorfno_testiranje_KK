#include "PrepIfVisitor.hpp"

/***********************
 * Shema transformacije
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
 ***********************/

/* Obrada odgovarajuceg ifa */
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

/* Provera da li je unutrasnji if */
bool PrepIfVisitor::imaIf(const Stmt *s) const {
    /* Nulta naredba nema prepreka */
    if (!s) return false;

    /* Provera naredbe kao ifa */
    if (isa<IfStmt>(s)) return true;

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        if (imaIf(dete)) return true;

    /* Inace nema prepreka */
    return false;
}

/* Priprema if naredbe */
bool PrepIfVisitor::VisitBreakStmt(BreakStmt *s) {
    /* Inicijalizacija deklaracije */
    DeclRefExpr *dekl = nullptr;

    /* Prolazak kroz roditelje tekuceg break */
    auto r = rods.at(s);
    for(;;) {
        /* Odustajanje ako je neka petlja ili switch */
        if (isa<DoStmt>(r)  || isa<WhileStmt>(r) ||
            isa<ForStmt>(r) || isa<SwitchStmt>(r))
            return true;

        /* Uzimanje roditelja koji je if */
        if (const auto rr = dyn_cast<IfStmt>(r)) {
            if (imaIf(rr->getThen())) return true;
            if (imaIf(rr->getElse())) return true;
            dekl = obradiIf(rr);
            break;
        }

        /* Nastavljanje dalje */
        r = rods.at(r);
    }

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
bool PrepIfVisitor::TraverseDecl(Decl *d) {
    /* Privatno cuvanje tekuce deklaracije */
    tekdek = d;

    /* Nastavljanje dalje */
    return MTKVisitor::TraverseDecl(d);
}
