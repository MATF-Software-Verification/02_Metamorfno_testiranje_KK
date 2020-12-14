#include "PrepSwitchVisitor.hpp"

#include "clang/AST/ParentMapContext.h"

/* Obrada odgovarajuceg switch */
DeclRefExpr *PrepSwitchVisitor::obradiSwitch(const SwitchStmt *s) {
    /* Odustajanje od pripremljenih */
    if (prip.count(s))
        return prip[s];

    /* Pronalazak prvog slobodnog imena */
    const auto ime = nadjiIme("cont");

    /* Deklaracija uslovne promenljive */
    const auto tip = TheASTContext.IntTy;
    auto dekl = napraviDecl(tekdek->getDeclContext(), tip, ime);

    /* Celobrojna nula za inicijalizaciju */
    dekl->setInit(napraviFalse());

    /* Naredba deklaracije uslovne promenljive */
    DeclStmt deknar{DeclGroupRef(dekl), SourceLocation(), SourceLocation()};

    /* Obmotavanje switcha deklaracijom */
    dodajIspred(s, &deknar);

    /* Continue za skok iz petlje */
    const auto cont = napraviCont();

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl, tip);

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
    const auto tip = TheASTContext.IntTy;
    const auto dodela = napraviDodelu(dekl, napraviTrue(), tip);

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
