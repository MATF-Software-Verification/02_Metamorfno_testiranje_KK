#include "PrepForVisitor.hpp"

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

/* Staticki podatak da li je prvi prolaz */
bool PrepForVisitor::prviProlaz = true;

/* Staticki podatak da li je imalo posla */
int PrepForVisitor::posaoBroj = 0;

/* Virtuelni dekstruktor za brojanje prolaza */
PrepForVisitor::~PrepForVisitor() {
    prviProlaz = false;
}

/* Dodavanje koraka for petlje pre continue */
bool PrepForVisitor::VisitContinueStmt(ContinueStmt *s) const {
    /* Dohvatanje for roditelja */
    const auto forr = dyn_cast<ForStmt>(petlje.top());
    if (!forr) return true;

    /* Dohvatanje koraka petlje */
    const auto korak = forr->getInc();
    if (!korak) return true;

    /* Pravljenje nove naredbe */
    const auto zamena = napraviSlozenu({korak, s});

    /* Tekstualna zamena koda */
    zameni(s, zamena);

    /* Nastavljanje dalje */
    return true;
}

/* Dohvatanje deklaracija */
void PrepForVisitor::dohvatiDeklaracije(Stmt *s) {
    /* Nulta naredba nema deklaracije */
    if (!s) return;

    /* Provera naredbe kao izraza deklaracije */
    if (const auto deklex = dyn_cast<DeclRefExpr>(s)) {
        /* Dodavanje tekuce imenovane deklaracije */
        dekls.insert(deklex->getDecl()->getDeclName().getAsString());

        /* Nastavljanje dalje */
        return;
    }

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        dohvatiDeklaracije(dete);
}

/* Odmaskiranje deklaracija */
void PrepForVisitor::odmaskirajDeklaracije(Stmt *s) {
    /* Nulta naredba nema deklaracije */
    if (!s) return;

    /* Provera naredbe i svih deklaracija u njoj */
    if (const auto deklst = dyn_cast<DeclStmt>(s)) {
        for (const auto dekl : deklst->getDeclGroup())
            /* Odmaskiranje deklaracije sa ponovljenim imenom */
            if (const auto val = dyn_cast<ValueDecl>(dekl)) {
                const auto ime = val->getDeclName().getAsString();

                /* Pronalazak novog imena i zamena starog */
                if (dekls.count(ime)) {
                    val->setDeclName(nadjiIdent(ime));
                }

                /* Ubacivanje novog imena u spisak koriscenih */
                dekls.insert(val->getDeclName().getAsString());
            }

        /* Nastavljanje dalje */
        return;
    }

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        odmaskirajDeklaracije(dete);
}

/* Odmaskiranje tela svake for petlje */
bool PrepForVisitor::VisitForStmt(ForStmt *s) {
    /* Odustajanje ako nije prvi prolaz */
    if (!prviProlaz) return true;

    /* Dohvatanje deklaracija u koraku */
    dohvatiDeklaracije(s->getInc());

    /* Odredjivanje mesta u kodu */
    const auto mesto = odrediMesto(s);

    /* Odmaskiranje deklaracija u telu */
    if (!dekls.empty())
        odmaskirajDeklaracije(s->getBody());

    /* Zamena petlje odmaskiranom verzijom */
    zameni(mesto, s);

    /* Praznjenje niza deklaracija */
    dekls.clear();

    /* Nastavljanje dalje */
    return true;
}

/* Pamcenje tekuce while petlje */
bool PrepForVisitor::TraverseWhileStmt(WhileStmt *s) {
    petlje.push(s);

    /* Obilazak petlje i dece */
    const auto rez = MTKVisitor::TraverseWhileStmt(s);

    /* Skidanje gotove petlje sa steka */
    petlje.pop();

    /* Nastavljanje dalje */
    return rez;
}

/* Pamcenje tekuce do petlje */
bool PrepForVisitor::TraverseDoStmt(DoStmt *s) {
    petlje.push(s);

    /* Obilazak petlje i dece */
    const auto rez = MTKVisitor::TraverseDoStmt(s);

    /* Skidanje gotove petlje sa steka */
    petlje.pop();

    /* Nastavljanje dalje */
    return rez;
}

/* Obilazak for petlje u zavisnosti od faze */
bool PrepForVisitor::TraverseForStmt(ForStmt *s) {
    petlje.push(s);

    /* Obilazak petlje i dece ako nije prvi prolaz */
    const auto rez = prviProlaz ? WalkUpFromForStmt(s) :
                     MTKVisitor::TraverseForStmt(s);

    /* Skidanje gotove petlje sa steka */
    petlje.pop();

    /* Nastavljanje dalje */
    return rez;
}

/* Podatak o tome da li je bilo vise prolaza */
bool PrepForVisitor::imaloPosla() {
    return posaoBroj++;
}
