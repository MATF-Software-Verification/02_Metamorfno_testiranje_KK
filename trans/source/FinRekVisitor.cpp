#include "FinRekVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * int petlja(int *x, int *y, int *ret) {
 *   for (; x > 2;) {
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
 * ---------------------
 * int petlja(int *x, int *y, int *ret) {
 *   if (*x > 2) {
 *     (*y)++;
 *     return 0;
 *     *x = *y;
 *     return petlja(x, y);
 *     int z;
 *     {
 *       *ret = 5;
 *       return 1;
 *     }
 *     return petlja(x, y);
 *   }
 *   return 0;
 * }
 ***********************/

/* Zamena svakog iskakanja returnom */
bool FinRekVisitor::VisitBreakStmt(BreakStmt *s) const {
    /* Potrebno je vratiti nulu */
    const auto ret = napraviReturn(false);

    /* Tekstualna zamena koda */
    zameni(s, ret, true);

    /* Nastavljanje dalje */
    return true;
}

/* Zamena svakog preskakanja returnom */
bool FinRekVisitor::VisitContinueStmt(ContinueStmt *s) const {
    /* Rekurzivni poziv funkcije */
    const auto call = napraviRekPoziv(tekf);

    /* Potrebno je vratiti poziv */
    const auto ret = napraviReturn(call);

    /* Tekstualna zamena koda */
    zameni(s, ret, true);

    /* Nastavljanje dalje */
    return true;
}

/* Zamena svakog return slozenom verzijom */
bool FinRekVisitor::VisitReturnStmt(ReturnStmt *s) const {
    /* Odustajanje ako je umetnuti return */
    const auto telo = dyn_cast<CompoundStmt>(tekf->getBody());
    if (s == telo->body_back()) return true;

    /* Deklaracija eventualne dodele */
    BinaryOperator *dodela = nullptr;

    /* Pravljenje ako ima povratne vrednosti */
    if (const auto pov = s->getRetValue()) {
        /* Deklaracija za povratnu vrednost */
        const auto dekl = tekf->parameters().back();

        /* Promenljiva za povratnu vrednost */
        const auto prom = napraviDeclExpr(dekl);

        /* Dereferencirana promenljiva */
        const auto deref = napraviDeref(prom);

        /* Dodela vrednosti promenljivoj */
        dodela = napraviDodelu(deref, pov);
    }

    /* Potrebno je vratiti jedinicu */
    Stmt *zamena = napraviReturn(true);

    /* Slozena naredba za spajanje */
    if (dodela) zamena = napraviSlozenu({dodela, zamena});

    /* Tekstualna zamena koda */
    zameni(s, zamena, true);

    /* Nastavljanje dalje */
    return true;
}

/* Obrada izraza na odgovarajuci nacin */
ParenExpr *FinRekVisitor::obradiIzraz(DeclRefExpr *s) {
    /* Odustajanje ako je vec posecen */
    if (izr.count(s)) return nullptr;

    /* Dodavanje u spisak obradjenih */
    izr.insert(s);

    /* Deklaracija od koje potice izraz */
    const auto dekl = s->getDecl();

    /* Odustajanje ako nije iz funkcije */
    if (!isa<ParmVarDecl>(dekl)) return nullptr;

    /* Dereferencirani izraz u zagradi */
    return napraviDeref(s);
}

/* Hvatanje izraza iz unarnog operatora */
bool FinRekVisitor::VisitUnaryOperator(UnaryOperator *s) {
    /* Proba da li je dete izraz deklaracije */
    const auto dekl = dyn_cast<DeclRefExpr>(s->getSubExpr());
    if (!dekl) return true;

    /* Dereferencirani izraz u zagradi */
    const auto zamena = obradiIzraz(dekl);
    if (!zamena) return true;

    /* Mesto izraza pre izmene */
    const auto mesto = odrediMesto(s, true);

    /* Izmena sa ukljucenim dereferenciranjem */
    s->setSubExpr(zamena);

    /* Tekstualna zamena koda */
    zameni(mesto, s);

    /* Nastavljanje dalje */
    return true;
}

/* Dereferenciranje svakog izraza iz petlje */
bool FinRekVisitor::VisitDeclRefExpr(DeclRefExpr *s) {
    /* Dereferencirani izraz u zagradi */
    const auto zamena = obradiIzraz(s);
    if (!zamena) return true;

    /* Tekstualna zamena koda */
    zameni(s, zamena, true);

    /* Nastavljanje dalje */
    return true;
}

/* Zamena gotovog for novom if naredbom */
bool FinRekVisitor::VisitForStmt(ForStmt *s) {
    /* Odustajanje ako for nije spreman */
    if (!fje->count(tekf->getName().str() + "_"))
        return true;

    /* Odgovarajuca if uslovna verzija */
    const auto iff = napraviIf(s->getCond(), s->getBody());

    /* Tekstualna zamena koda */
    zameni(s, iff);

    /* Izbacivanje glavne funkcije skupa */
    fje->erase(tekf->getName().str());

    /* Nastavljanje dalje */
    return s;
}

/* Obrada svake (for) petlje ili ne */
bool FinRekVisitor::TraverseForStmt(ForStmt *s) {
    return fje->count(tekf->getName().str() + "_") ? WalkUpFromForStmt(s) :
           RecursiveASTVisitor<FinRekVisitor>::TraverseForStmt(s);
}

/* Obrada deklaracije funkcije */
bool FinRekVisitor::TraverseFunctionDecl(FunctionDecl *f) {
    /* Privatno cuvanje tekuce funkcije */
    tekf = f;

    /* Nastavljanje dalje ako je funkcija na spisku */
    if (fje->count(f->getName().str())) {
        const auto ret =
            RecursiveASTVisitor<FinRekVisitor>::TraverseFunctionDecl(f);

        /* Dodavanje funkcije u skup spremnih */
        fje->insert(f->getName().str() + "_");

        /* Vracanje rezultata */
        return ret;
    /* Povlacenje nazad ako funkcija nije na spisku */
    } else return WalkUpFromFunctionDecl(f);
}
