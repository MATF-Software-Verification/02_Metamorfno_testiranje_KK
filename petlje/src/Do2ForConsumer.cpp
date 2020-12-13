#include "Do2ForConsumer.hpp"

/* Posetilac koji do pretvara u for */
bool Do2ForVisitor::VisitDoStmt(DoStmt *s) {
    /* Pomocne promenljive za pretragu */
    std::string cond("cond"), ime(cond);
    auto i = 0ull; /* brojac */

    /* Pronalazak prvog slobodnog imena */
    while (TheASTContext.Idents.find(ime)
           != TheASTContext.Idents.end()) {
        if (i == ULLONG_MAX) exit(EXIT_FAILURE);
        ime = cond + std::to_string(i++);
    }

    /* Deklaracija uslovne promenljive */
    const auto tip = TheASTContext.IntTy;
    auto *dekl = VarDecl::Create(TheASTContext, tekdek->getDeclContext(),
                                 SourceLocation(), SourceLocation(),
                                 &TheASTContext.Idents.getOwn(ime),
                                 tip, nullptr, SC_None);

    /* Celobrojna jedinica za inicijalizaciju */
    llvm::APInt APValue(static_cast<unsigned>(TheASTContext.getTypeSize(tip)), 1);
    dekl->setInit(IntegerLiteral::Create(TheASTContext, APValue,
                                         tip, SourceLocation()));

    /* Naredba deklaracije uslovne promenljive */
    DeclStmt deknar{DeclGroupRef(dekl), SourceLocation(), SourceLocation()};

    /* Uslovna promenljiva kao izraz */
    const auto uslov = DeclRefExpr::Create(TheASTContext, NestedNameSpecifierLoc(),
                                           SourceLocation(), dekl, true,
                                           SourceLocation(), tip, VK_LValue);

    /* Korak kao dodela uslova promenljivoj */
    const auto korak =
        BinaryOperator::Create(TheASTContext,
                               uslov, s->getCond(), BO_Assign,
                               tip, VK_RValue, OK_Ordinary,
                               SourceLocation(), FPOptionsOverride());

    /* Formiranje nove for petlje */
    ForStmt petlja(TheASTContext, nullptr, uslov, nullptr, korak, s->getBody(),
                   SourceLocation(), SourceLocation(), SourceLocation());

    /* Slozena naredba kao resenje */
    Stmt *resenje = CompoundStmt::Create(TheASTContext,
                        std::vector<Stmt *>{&deknar, &petlja},
                        SourceLocation(), SourceLocation());

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
