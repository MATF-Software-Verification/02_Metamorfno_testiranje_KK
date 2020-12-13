#include "For2DoVisitor.hpp"

/* Posetilac koji for pretvara u do */
bool For2DoVisitor::VisitForStmt(ForStmt *s) {
    /* Slozena naredba sa telom i inkrementacijom
     * ili samo telo ako nema inkrementacije */
    Stmt *telo;
    if (s->getInc()) {
      telo = CompoundStmt::Create(TheASTContext,
                 std::vector<Stmt *>{s->getBody(), s->getInc()},
                 SourceLocation(), SourceLocation());
    } else {
      telo = s->getBody();
    }

    /* Do petlja sa novim telom i uslovom
     * ili beskonacna petlja ako nema uslova */
    Expr *cond = s->getCond();
    if (!cond) {
      const auto tip = TheASTContext.IntTy;
      llvm::APInt APValue(static_cast<unsigned>(TheASTContext.getTypeSize(tip)), 1);
      cond = IntegerLiteral::Create(TheASTContext, APValue,
                                    tip, SourceLocation());
    }
    DoStmt petlja(telo, cond, SourceLocation(),
                  SourceLocation(), SourceLocation());

    /* If naredba za proveru uslova petlje */
    const auto uslov =
        IfStmt::Create(TheASTContext, SourceLocation(),
                       false, nullptr, nullptr, cond, &petlja);

    /* Slozena naredba sa inicijalizacijom i petljom
     * ili samo petlja ako nema inicijalizacije */
    Stmt *initpet;
    if (s->getInit()) {
      initpet = CompoundStmt::Create(TheASTContext,
                    std::vector<Stmt *>{s->getInit(), uslov},
                    SourceLocation(), SourceLocation());
    } else {
      initpet = uslov;
    }

    /* Tekstualna zamena koda */
    zameni(s, initpet);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod for petlje */
bool For2DoVisitor::TraverseForStmt(ForStmt *s) {
    return WalkUpFromForStmt(s);
}
