#include "Iter2RekVisitor.hpp"

/****************************************
 * Shema transformacije
 * --------------------------------------
 * while (x > 2) {
 *   y++;
 *   break;
 *   x = y;
 *   continue;
 *   int z;
 *   return 5;
 * }
 * --------------------------------------
 * int petlja(int *x, int *y, int *ret) {
 *   if (x > 2) {
 *     y++;
 *     break;
 *     x = y;
 *     continue;
 *     int z;
 *     return 5;
 *     continue;
 *   }
 *   return 0;
 * }
 *
 * int ret;
 * if (rek(&x, &y, &ret))
 *   return ret;
 ****************************************/

/* Provera ima li prepreka za izmenu */
bool Iter2RekVisitor::imaWhilePrepreka(Stmt *s) const {
    /* Nulta naredba nema prepreka */
    if (!s) return false;

    /* Provera naredbe kao while petlje */
    if (isa<WhileStmt>(s))
        return true;

    /* Provera naredbe kao oznake */
    if (isa<LabelStmt>(s))
        return true;

    /* Provera naredbe kao skoka */
    if (isa<GotoStmt>(s))
        return true;

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        if (imaWhilePrepreka(dete))
            return true;

    /* Inace nema prepreka */
    return false;
}

/* Provera ima li prepreka za izmenu */
bool Iter2RekVisitor::imaWhilePrepreka(WhileStmt *s) const {
    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        if (imaWhilePrepreka(dete))
            return true;

    /* Inace nema prepreka */
    return false;
}

/* Dohvatanje deklaracija na koje se referise */
void Iter2RekVisitor::dohvatiDeklaracije(Stmt *s) {
    /* Nulta naredba nema deklaracije */
    if (!s) return;

    /* Provera naredbe kao deklaracije */
    if (const auto deklst = dyn_cast<DeclStmt>(s)) {
        /* Prolazak kroz sve deklaracije */
        for (const auto dekl : deklst->getDeclGroup())
            /* Popunjavanje tabu skupa */
            if (const auto var = dyn_cast<VarDecl>(dekl)) {
                tabu.insert(var);

                /* Dohvatanje dodatnih deklaracija */
                dohvatiDeklaracije(var->getInit());
            }

        /* Nastavljanje dalje */
        return;
    }

    /* Provera naredbe kao izraza deklaracije */
    if (const auto deklex = dyn_cast<DeclRefExpr>(s)) {
        /* Dodavanje svih deklaracija */
        if (const auto var = dyn_cast<VarDecl>(deklex->getDecl()))
            if (!tabu.count(var) && !deklm.count(var)) {
                deklm.insert(var);
                dekls.push_back(var);
            }

        /* Nastavljanje dalje */
        return;
    }

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        dohvatiDeklaracije(dete);
}

/* Pravljenje rekurzivne funkcije */
FunctionDecl *Iter2RekVisitor::napraviRekFunkciju(WhileStmt *s) const {
    /* Kontekst deklaracije visi od fje */
    const auto dekl = tekf->getParent();

    /* Funkcija vraca celobrojnu vrednost */
    const auto tip = TheASTContext.IntTy;

    /* Svodjenje petlje na uslovnu naredbu */
    const auto iff = napraviIf(s->getCond(), s->getBody());
    iff->setThen(napraviSlozenu({iff->getThen(), napraviCont()}));

    /* Pravljenje naredbe za vracanje iz fje */
    const auto ret = napraviReturn(false);

    /* Pravljenje tela zeljene funkcije */
    const auto telo = napraviSlozenu({iff, ret});

    /* Pravljenje ref verzije funkcije */
    return napraviRefFunkciju(dekl, tip, "petlja", dekls, telo);
}

/* Eliminisanje svake (while) petlje */
bool Iter2RekVisitor::VisitWhileStmt(WhileStmt *s) {
    /* Odustajanje ako nije okej */
    if (imaWhilePrepreka(s)) return true;

    /* Dohvatanje deklaracija na koje while referise */
    dohvatiDeklaracije(s);

    /* Deklaracija povratne vrednosti ako ima smisla */
    DeclStmt *ret = nullptr;
    if (!tekf->getReturnType()->isVoidType())
        ret = napraviDeclStmt(tekf, "ret", tekf->getReturnType());

    /* Dodavanje povratne vrednosti u spisak parametara */
    const auto retex = ret ? napraviDeclExpr(ret) : nullptr;
    if (retex) dekls.push_back(dyn_cast<VarDecl>(retex->getDecl()));

    /* Pravljenje odgovarajuce funkcije */
    const auto fja = napraviRekFunkciju(s);

    /* Tekstualna dopuna koda funkcijom */
    dodajFunkciju(tekf, fja);

    /* Cuvanje njenog imena za kasnije */
    fje->insert(fja->getName().str());

    /* Vracanje tipa povratne vrednosti u normalnu */
    if (ret) dekls.back()->setType(retex->getType());

    /* Poziv napravljene funkcije */
    const auto call = napraviRefPoziv(fja, dekls);

    /* Vracanje iz pozivaoca ako postoji potreba */
    const auto rtrn = napraviReturn(retex);

    /* Uslovna naredba rekurzivne funkcije */
    const auto iff = napraviIf(call, rtrn);

    /* Slozena naredba kao zamena za petlju */
    Stmt *zamena = iff;
    if (ret) zamena = napraviSlozenu({ret, iff});

    /* Tekstualna zamena koda */
    zameni(s, zamena);

    /* Praznjenje zapamcenih deklaracija */
    dekls.clear();
    deklm.clear();
    tabu.clear();

    /* Nastavljanje dalje */
    return true;
}

/* Pamcenje tekuce funkcije */
bool Iter2RekVisitor::VisitFunctionDecl(FunctionDecl *f) {
    tekf = f;

    /* Nastavljanje dalje */
    return true;
}

/* Prolazak kroz sve (while) petlje */
bool Iter2RekVisitor::TraverseWhileStmt(WhileStmt *s) {
    /* Nastavljanje dalje ili povlacenje nazad */
    return !imaWhilePrepreka(s) ? WalkUpFromWhileStmt(s) :
           RecursiveASTVisitor<Iter2RekVisitor>::TraverseWhileStmt(s);
}
