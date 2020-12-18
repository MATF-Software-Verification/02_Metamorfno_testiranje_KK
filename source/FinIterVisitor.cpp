#include "FinIterVisitor.hpp"

/******************************
 * Shema transformacije
 * ----------------------------
 * fja(a, ..., b) {
 *   while (!uslov) {
 *     telo1;
 *     return rek(a0, ..., b0);
 *     telo2;
 *     return rek(a0, ..., b0);
 *   }
 *   telo3;
 *   return baza;
 * }
 * ----------------------------
 * fja(a, ..., b) {
 *   while (!uslov) {
 *     telo1;
 *     a0 = ...; b0 = ...;
 *     a = a0; b = b0;
 *     continue;
 *     telo2;
 *     a0 = ...; b0 = ...;
 *     a = a0; b = b0;
 *     continue;
 *   }
 *   telo3;
 *   return baza;
 * }
 ******************************/

/* Zamena svakog kriticnog return */
bool FinIterVisitor::VisitReturnStmt(ReturnStmt *s) const {
    /* Dohvatanje poziva iz povratne vrednosti */
    const auto call = dyn_cast_or_null<CallExpr>(s->getRetValue());

    /* Odustajanje ako nije tekuca funkcija */
    if (!call || call->getDirectCallee() != tekf) return true;

    /* Dohvatanje svih parametara */
    const auto parms = tekf->parameters();

    /* Dohvatanje svih argumenata */
    const auto args = call->getRawSubExprs();

    /* Inicijalizacija niza naredbi */
    std::vector<Stmt *> naredbe;

    /* Popunjavanje niza deklaracijama */
    for (auto i = 0ul; i < parms.size(); i++) {
        const auto tip = parms[i]->getType();
        const auto ime = parms[i]->getName().str();
        const auto pocetna = cast<Expr>(args[i+1]);
        naredbe.push_back(napraviDeclStmt(tekf, ime, tip, pocetna));
    }

    /* Popunjavanje niza dodelama */
    for (auto i = 0ul; i < parms.size(); i++) {
        const auto levo = napraviDeclExpr(parms[i]);
        const auto deknar = cast<DeclStmt>(naredbe[i]);
        const auto desno = napraviDeclExpr(deknar);
        naredbe.push_back(napraviDodelu(levo, desno));
    }

    /* Dodavanje continue za nastavak */
    naredbe.push_back(napraviCont());

    /* Slozena naredba za zamenu */
    const auto zamena = napraviSlozenu(naredbe);

    /* Tekstualna zamena koda */
    zameni(s, zamena);

    /* Nastavljanje dalje */
    return true;
}

/* Obrada deklaracije funkcije */
bool FinIterVisitor::TraverseFunctionDecl(FunctionDecl *f) {
    /* Privatno cuvanje tekuce funkcije */
    tekf = f;

    /* Nastavljanje dalje ili povlacenje nazad */
    return !fje->count(f->getName().str()) ? WalkUpFromFunctionDecl(f) :
           RecursiveASTVisitor<FinIterVisitor>::TraverseFunctionDecl(f);
}
