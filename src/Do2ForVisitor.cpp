#include "Do2ForVisitor.hpp"

/* Posetilac koji do pretvara u for */
bool Do2ForVisitor::VisitDoStmt(DoStmt *s) {
    /* Pronalazak prvog slobodnog imena */
    const auto ime = nadjiIme("cond");

    /* Deklaracija uslovne promenljive */
    const auto tip = TheASTContext.IntTy;
    auto dekl = napraviDecl(tekdek->getDeclContext(), tip, ime);

    /* Celobrojna jedinica za inicijalizaciju */
    dekl->setInit(napraviTrue());

    /* Naredba deklaracije uslovne promenljive */
    DeclStmt deknar{DeclGroupRef(dekl), SourceLocation(), SourceLocation()};

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl, tip);

    /* Korak kao dodela uslova promenljivoj */
    const auto korak = napraviDodelu(uslov, s->getCond(), tip);

    /* Formiranje nove for petlje */
    const auto petlja = napraviFor(uslov, korak, s->getBody());

    /* Slozena naredba kao resenje */
    const auto resenje = napraviSlozenu({&deknar, petlja});

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
