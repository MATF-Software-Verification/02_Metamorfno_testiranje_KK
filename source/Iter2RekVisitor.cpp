#include "Iter2RekVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 *
 * ---------------------
 *
 ***********************/

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
        if (imaWhilePrepreka(dete)) return true;

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
            if (!tabu.count(var)) dekls.insert(var);

        /* Nastavljanje dalje */
        return;
    }

    /* Prolazak kroz svu decu */
    for (const auto dete : s->children())
        dohvatiDeklaracije(dete);
}

/* Eliminisanje svake (while) petlje */
bool Iter2RekVisitor::VisitWhileStmt(WhileStmt *s) {
    /* Odustajanje ako nije okej */
    if (imaWhilePrepreka(s)) return true;

    /* Dohvatanje deklaracija na koje while referise */
    dohvatiDeklaracije(s);

    /* Praznjenje zapamcenih deklaracija */
    dekls.clear();
    tabu.clear();

    /* Nastavljanje dalje */
    return true;
}

/* Prolazak kroz sve (while) petlje */
bool Iter2RekVisitor::TraverseWhileStmt(WhileStmt *s) {
    /* Nastavljanje dalje ili povlacenje nazad */
    return !imaWhilePrepreka(s) ? WalkUpFromWhileStmt(s) :
           RecursiveASTVisitor<Iter2RekVisitor>::TraverseWhileStmt(s);
}
