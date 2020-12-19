#include "Rek2IterVisitor.hpp"

/******************************
 * Shema transformacije
 * ----------------------------
 * fja(a, ..., b) {
 *   if (uslov) {
 *     telo1;
 *     return baza;
 *   } else {
 *     telo2;
 *     return rek(a0, ..., b0);
 *   }
 * }
 * ----------------------------
 * fja(a, ..., b) {
 *   while (!uslov) {
 *     telo2;
 *     return rek(a0, ..., b0);
 *   }
 *   telo1;
 *   return baza;
 * }
 ******************************/

/* Odredjivanje return statusa */
Rek2IterVisitor::Status Rek2IterVisitor::retStatus(Stmt *s) const {
    /* Nulte naredbe nemaju return */
    if (!s) return Status::NIJE_RETURN;

    /* Obrada bas return naredbe */
    if (const auto ret = dyn_cast<ReturnStmt>(s)) {
        /* Obrada izraza poziva */
        if (const auto call = dyn_cast_or_null<CallExpr>(ret->getRetValue())) {
            if (call->getDirectCallee() == tekf)
                return Status::REK_RETURN;
            else return Status::NEREK_RETURN;
        /* Nerekurzivan return ako nije poziv */
        } else return Status::NEREK_RETURN;
    }

    /* Rekurzivna obrada slozene naredbe */
    if (const auto comp = dyn_cast<CompoundStmt>(s))
        return retStatus(comp->body_back());

    /* Nije return ako se dovde doslo */
    return Status::NIJE_RETURN;
}

/* Provera da li je baza rekurzivna */
bool Rek2IterVisitor::rekurzivnaBaza(Stmt *s) const {
    /* Nulta naredba nije rekurzivna */
    if (!s) return false;

    /* Provera naredbe kao poziva */
    if (const auto call = dyn_cast<CallExpr>(s))
        return call->getDirectCallee() == tekf;

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        if (rekurzivnaBaza(dete)) return true;

    /* Inace nije rekurzivna naredba */
    return false;
}

/* Pravljenje odgovarajuce zamene */
CompoundStmt *Rek2IterVisitor::napraviZamenu(Expr *uslov, Stmt *iter,
                                             Stmt *baza, bool neg) const {
    /* Odustajanje ako je baza rekurzivna */
    if (rekurzivnaBaza(baza)) return nullptr;

    /* Negacija uslova ako treba */
    if (neg) uslov = napraviNegaciju(uslov);

    /* Iteracija kroz rekurzivni deo */
    const auto petlja = napraviWhile(uslov, iter);

    /* Dodavanje baze iza iteracije */
    return napraviSlozenu({petlja, baza});
}

/* Obrada nerek-then i rek-else */
Stmt *Rek2IterVisitor::obradiITENR() const {
    /* Izracunavanje zamene */
    return napraviZamenu(iff->getCond(), iff->getElse(),
                         iff->getThen(), true);
}

/* Obrada rek-then i nerek-else */
Stmt *Rek2IterVisitor::obradiITERN() const {
    /* Izracunavanje zamene */
    return napraviZamenu(iff->getCond(), iff->getThen(),
                         iff->getElse(), false);
}

/* Obrada slucaja if-then-else */
Stmt *Rek2IterVisitor::obradiIfThenElse() const {
    /* Podaci o povratnim vrednostima */
    const auto thens = retStatus(iff->getThen());
    const auto elses = retStatus(iff->getElse());

    /* Grananje na tri podslucaja */
    if (thens == Status::NEREK_RETURN && elses == Status::REK_RETURN)
        return obradiITENR();
    else if (thens == Status::REK_RETURN && elses == Status::NEREK_RETURN)
        return obradiITERN();
    /* Posebna obrada void funkcija */
    else if (!tekf->getReturnType()->isVoidType()) return nullptr;

    /* Grananje na tri podslucaja */
    if (thens == Status::NIJE_RETURN && elses == Status::REK_RETURN)
        return obradiITENR();
    else if (thens == Status::REK_RETURN && elses == Status::NIJE_RETURN)
        return obradiITERN();
    else return nullptr;
}

/* Sakupljanje dece iz opsega */
Stmt *Rek2IterVisitor::sakupiDecu(StmtIterator start, StmtIterator end) const {
    std::vector<Stmt *> deca;
    for (auto dete = start; dete != end; dete ++)
        deca.push_back(*dete);
    return deca.size() == 1 ? deca.front() : napraviSlozenu(deca);
}

/* Obrada nerek-then i rek-tela */
Stmt *Rek2IterVisitor::obradiITTNR() const {
    /* Sakupljanje dece */
    const auto deca = sakupiDecu(
                          std::next(telo->body_begin()),
                                    telo->body_end()
                      );

    /* Izracunavanje zamene */
    return napraviZamenu(iff->getCond(), deca,
                         iff->getThen(), true);
}

/* Obrada rek-then i nerek-tela */
Stmt *Rek2IterVisitor::obradiITTRN() const {
    /* Sakupljanje dece */
    const auto deca = sakupiDecu(
                          std::next(telo->body_begin()),
                                    telo->body_end()
                      );

    /* Izracunavanje zamene */
    return napraviZamenu(iff->getCond(),
                         iff->getThen(),
                         deca, false);
}

/* Obrada slucaja if-then-telo */
Stmt *Rek2IterVisitor::obradiIfThenTelo() const {
    /* Odustajanje ako postoji else */
    if (iff->getElse()) return nullptr;

    /* Podaci o povratnim vrednostima */
    const auto thens = retStatus(iff->getThen());
    const auto telos = retStatus(telo);

    /* Grananje na tri podslucaja */
    if (thens == Status::NEREK_RETURN && telos == Status::REK_RETURN)
        return obradiITTNR();
    else if (thens == Status::REK_RETURN && telos == Status::NEREK_RETURN)
        return obradiITTRN();
    /* Posebna obrada void funkcija */
    else if (!tekf->getReturnType()->isVoidType()) return nullptr;

    /* Grananje na dva podslucaja */
    if (thens == Status::REK_RETURN && telos == Status::NIJE_RETURN)
        return obradiITTRN();
    else return nullptr;
}

/* Obrada deklaracije funkcije */
bool Rek2IterVisitor::VisitFunctionDecl(FunctionDecl *f) {
    /* Pamcenje tekuce funkcije */
    tekf = f;

    /* Neprazno telo tekuce funkcije */
    telo = dyn_cast_or_null<CompoundStmt>(f->getBody());
    if (!telo || telo->body_empty()) return true;

    /* Dohvatanje prvog deteta */
    iff = dyn_cast<IfStmt>(telo->body_front());
    if (!iff) return true;

    /* Tekstualna zamena koda */
    if (const auto zamena = (iff == telo->body_back()) ?
                             obradiIfThenElse() : obradiIfThenTelo()) {
        zameni(telo, zamena);

        /* Dodavanje u spisak obradjenih */
        fje->insert(f->getName().str());
    }

    /* Nastavljanje dalje */
    return true;
}
