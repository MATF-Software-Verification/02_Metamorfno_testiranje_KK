#include "Do2ForVisitor.hpp"

/* Posetilac koji do pretvara u for */
bool Do2ForVisitor::VisitDoStmt(DoStmt *s) {
    /* Deklaracija uslovne promenljive */
    const auto dekl = napraviUslovnu(tekdek, "cond", true);

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl);

    /* Korak kao dodela uslova promenljivoj */
    const auto korak = napraviDodelu(uslov, s->getCond());

    /* Formiranje nove for petlje */
    const auto petlja = napraviFor(uslov, korak, s->getBody());

    /* Slozena naredba kao resenje */
    const auto resenje = napraviSlozenu({dekl, petlja});

    /* Tekstualna zamena koda */
    zameni(s, resenje);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod do petlje */
bool Do2ForVisitor::TraverseDoStmt(DoStmt *s) {
    return WalkUpFromDoStmt(s);
}

/* Nacin obrade deklaracije */
bool Do2ForVisitor::TraverseDecl(Decl *d) {
    /* Privatno cuvanje tekuce deklaracije */
    tekdek = d;

    /* Nastavljanje dalje */
    return RecursiveASTVisitor<Do2ForVisitor>::TraverseDecl(d);
}
