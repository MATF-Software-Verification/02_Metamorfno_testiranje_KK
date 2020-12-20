#include "PrepForVisitor.hpp"

#include "clang/AST/ParentMapContext.h"

/*************************
 * Shema transformacije
 * -----------------------
 * for (init; cond; inc) {
 *   telo1;
 *   continue;
 *   telo2;
 * }
 * -----------------------
 * for (init; cond; inc) {
 *   telo1;
 *   {
 *     inc;
 *     continue;
 *   }
 *   telo2;
 * }
 *************************/

/* Poruka da maskiranje koraka pravi problem */
static constexpr auto maskiranje = "Telo petlje maskira korak!";

/* Odredjivanje petlje i prethodnog */
std::pair<const ForStmt *,
          const Stmt *> PrepForVisitor::odrediPetlju(Stmt *s) const {
    /* Inicijalizacija for petlje */
    const ForStmt *forr = nullptr;

    /* Cuvanje prethodnog roditelja */
    const Stmt *pret = s;

    /* Pronalazak roditelja koji je for */
    auto rod = TheASTContext.getParents(*s);
    while (!rod.empty()) {
        /* Izdvajanje glavnog roditelja */
        const auto r = rod.begin()->get<Stmt>();
        if (!r) break;

        /* Provera da li je for */
        forr = dyn_cast<ForStmt>(r);
        if (forr) break;

        /* Cuvanje prethodnog roditelja */
        pret = r;

        /* Nastavljanje dalje */
        rod = TheASTContext.getParents(*r);
    }

    /* Vracanje rezultata */
    return std::make_pair(forr, pret);
}

/* Pronalazak svih imena u koraku */
bool PrepForVisitor::VisitDeclRefExpr(DeclRefExpr *s) {
    /* Deklaracija petlje */
    const ForStmt *forr;
    const Stmt *pret;

    /* Dohvatanje petlje */
    std::tie(forr, pret) = odrediPetlju(s);

    /* Odustajanje ako nije for i inc */
    if (!forr || pret != forr->getInc())
        return true;

    /* Dohvatanje tekuce deklaracije */
    const auto dekl = dyn_cast<NamedDecl>(s->getDecl());
    if (!dekl) return true;

    /* Ime tekuce deklaracije */
    const auto ime = dekl->getName().str();

    /* Dodavanje imena u spisak */
    imena[forr].insert(ime);

    /* Nastavljanje dalje */
    return true;
}

/* Pronalazak svih imena u telu petlje */
bool PrepForVisitor::VisitDeclStmt(DeclStmt *s) {
    /* Deklaracija petlje */
    const ForStmt *forr;
    const Stmt *pret;

    /* Dohvatanje petlje */
    std::tie(forr, pret) = odrediPetlju(s);

    /* Odustajanje ako nije for ili je init */
    if (!forr || pret == forr->getInit())
        return true;

    /* Prolazak kroz sve deklaracije */
    for (const auto dekl : s->getDeclGroup())
        /* Dohvatanje deklaracije promenljive */
        if (const auto var = dyn_cast<VarDecl>(dekl)) {
            /* Ime tekuce deklaracije promenljive */
            const auto ime = var->getName().str();

            /* Azuriranje podatka o maskiranosti */
            if (imena[forr].count(ime))
                maskirani.insert(forr);
        }

    /* Nastavljanje dalje */
    return true;
}

/* Dodavanje koraka for petlje pre continue */
bool PrepForVisitor::VisitContinueStmt(ContinueStmt *s) const {
    /* Deklaracija petlje */
    const ForStmt *forr = nullptr;

    /* Inicijalizacija nove naredbe */
    Stmt *stmt = s;

    /* Prolazak kroz roditelje tekuceg continue */
    auto rod = TheASTContext.getParents(*s);
    while (!rod.empty()) {
        /* Izdvajanje glavnog roditelja */
        const auto r = rod.begin()->get<Stmt>();

        /* Odustajanje ako je do ili while */
        if (isa<DoStmt>(r) || isa<WhileStmt>(r))
            return true;

        /* Uzimanje roditelja koji je for */
        if ((forr = dyn_cast<ForStmt>(r))) {
            /* Odustajanje ako nema korak */
            if (!forr->getInc())
                return true;

            /* Pravljenje nove naredbe */
            auto inc = const_cast<Stmt *>(cast<Stmt>(forr->getInc()));
            stmt = napraviSlozenu({inc, s});
            break;
        }

        /* Nastavljanje dalje */
        rod = TheASTContext.getParents(*r);
    }

    /* Greska ukoliko petlja maskira korak */
    if (maskirani.count(forr))
        greska(maskiranje);

    /* Tekstualna zamena koda */
    zameni(s, stmt);

    /* Nastavljanje dalje */
    return true;
}
