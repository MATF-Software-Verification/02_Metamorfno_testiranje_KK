#include "LoopUnrollVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * while (uslov)
 *   telo;
 * ---------------------
 * while (uslov) {
 *   telo;
 *   if (!uslov)
 *     break;
 *   telo;
 *   if (!uslov)
 *     break;
 *   telo;
 * }
 ***********************/

/* Staticki podatak o broju odmotavanja */
unsigned long long LoopUnrollVisitor::n = 0;

/* Postavljanje broja odmotavanja */
void LoopUnrollVisitor::postaviBroj(unsigned long long i) {
    n = i;
}

/* Umnozavanje tela do petlje */
void LoopUnrollVisitor::umnoziTelo(DoStmt *s) const {
    /* Telo tekuce petlje */
    const auto telo = s->getBody();
    if (isa<NullStmt>(telo)) return;

    /* Niz naredbi novog tela */
    std::vector<Stmt *> novoTelo{telo};

    /* Negacija uslova petlje */
    const auto uslov = napraviNegaciju(s->getCond());

    /* Naredba za iskakanje */
    const auto br = napraviBreak();

    /* Iskakanje ako nije uspunjen uslov */
    const auto skok = napraviIf(uslov, br);

    /* Dodavanje na telo koliko treba */
    for (auto i = 0ull; i < n; i++) {
        novoTelo.push_back(skok);
        novoTelo.push_back(telo);
    }

    /* Slozena naredba kao rezultat */
    const auto rez = napraviSlozenu(novoTelo);

    /* Postavljanje novog tela */
    s->setBody(rez);
}

/* Umnozavanje tela while petlje */
void LoopUnrollVisitor::umnoziTelo(WhileStmt *s) const {
    /* Telo tekuce petlje */
    const auto telo = s->getBody();
    if (isa<NullStmt>(telo)) return;

    /* Niz naredbi novog tela */
    std::vector<Stmt *> novoTelo{telo};

    /* Negacija uslova petlje */
    const auto uslov = napraviNegaciju(s->getCond());

    /* Naredba za iskakanje */
    const auto br = napraviBreak();

    /* Iskakanje ako nije uspunjen uslov */
    const auto skok = napraviIf(uslov, br);

    /* Dodavanje na telo koliko treba */
    for (auto i = 0ull; i < n; i++) {
        novoTelo.push_back(skok);
        novoTelo.push_back(telo);
    }

    /* Slozena naredba kao rezultat */
    const auto rez = napraviSlozenu(novoTelo);

    /* Postavljanje novog tela */
    s->setBody(rez);
}

/* Umnozavanje tela for petlje */
void LoopUnrollVisitor::umnoziTelo(ForStmt *s) const {
    /* Telo tekuce petlje */
    auto telo = s->getBody();
    if (isa<NullStmt>(telo)) telo = nullptr;

    /* Niz naredbi novog tela */
    std::vector<Stmt *> novoTelo;
    if (telo) novoTelo.push_back(telo);

    /* Negacija uslova petlje ako ga ima */
    Expr *uslov = s->getCond();
    if (uslov) uslov = napraviNegaciju(uslov);

    /* Naredba za iskakanje */
    const auto br = napraviBreak();

    /* Iskakanje ako nije uspunjen uslov */
    Stmt *skok = nullptr;
    if (uslov) skok = napraviIf(uslov, br);

    /* Korak tekuce petlje */
    const auto korak = s->getInc();

    /* Dodavanje na telo koliko treba */
    for (auto i = 0ull; i < n; i++) {
        if (korak) novoTelo.push_back(korak);
        if (skok) novoTelo.push_back(skok);
        if (telo) novoTelo.push_back(telo);
    }

    /* Slozena naredba kao rezultat */
    if (!novoTelo.empty()) {
        const auto rez = napraviSlozenu(novoTelo);

        /* Postavljanje novog tela */
        s->setBody(rez);
    }
}

/* Obrada dece svake najvise petlje */
void LoopUnrollVisitor::obradiDecu(Stmt *s) const {
    /* Nulta naredba nema dece */
    if (!s) return;

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        obradiDecu(dete);

    /* Provera naredbe kao do petlje */
    if (const auto petlja = dyn_cast<DoStmt>(s))
        umnoziTelo(petlja);

    /* Provera naredbe kao while petlje */
    if (const auto petlja = dyn_cast<WhileStmt>(s))
        umnoziTelo(petlja);

    /* Provera naredbe kao for petlje */
    if (const auto petlja = dyn_cast<ForStmt>(s))
        umnoziTelo(petlja);
}

/* Provera ima li prepreka za izmenu */
bool LoopUnrollVisitor::imaLabelStmt(Stmt *s) const {
    /* Nulta naredba nema oznake */
    if (!s) return false;

    /* Provera naredbe kao oznake */
    if (isa<LabelStmt>(s))
        return true;

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        if (imaLabelStmt(dete))
            return true;

    /* Inace nema prepreka */
    return false;
}

/* Poseta svake petlje na isti nacin */
bool LoopUnrollVisitor::posetiPetlju(Stmt *s) const {
    /* Odustajanje ako ima oznake u telu */
    if (imaLabelStmt(s)) return true;

    /* Odredjivanje mesta u kodu */
    const auto mesto = odrediMesto(s);

    /* Obrada sve dece tekuce petlje */
    obradiDecu(s);

    /* Zamena odmotanom verzijom */
    zameni(mesto, s);

    /* Nastavljanje dalje */
    return true;
}

/* Umnozavanje tela svake do petlje */
bool LoopUnrollVisitor::VisitDoStmt(DoStmt *s) const {
    return posetiPetlju(s);
}

/* Umnozavanje tela svake while petlje */
bool LoopUnrollVisitor::VisitWhileStmt(WhileStmt *s) const {
    return posetiPetlju(s);
}

/* Umnozavanje tela svake for petlje */
bool LoopUnrollVisitor::VisitForStmt(ForStmt *s) const {
    return posetiPetlju(s);
}

/* Prekid obilaska kod prve do petlje */
bool LoopUnrollVisitor::TraverseDoStmt(DoStmt *s) {
    return !imaLabelStmt(s) ? WalkUpFromDoStmt(s) :
           MTKVisitor::TraverseDoStmt(s);
}

/* Prekid obilaska kod prve while petlje */
bool LoopUnrollVisitor::TraverseWhileStmt(WhileStmt *s) {
    return !imaLabelStmt(s) ? WalkUpFromWhileStmt(s) :
           MTKVisitor::TraverseWhileStmt(s);
}

/* Prekid obilaska kod prve for petlje */
bool LoopUnrollVisitor::TraverseForStmt(ForStmt *s) {
    return !imaLabelStmt(s) ? WalkUpFromForStmt(s) :
           MTKVisitor::TraverseForStmt(s);
}
