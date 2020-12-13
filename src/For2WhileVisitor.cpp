#include "For2WhileVisitor.hpp"
  
/* Posetilac koji for pretvara u while */
bool For2WhileVisitor::VisitForStmt(ForStmt *s) {
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

    /* While petlja sa novim telom i uslovom
     * ili beskonacna petlja ako nema uslova */
    Expr *cond = s->getCond();
    if (!cond) {
      const auto tip = TheASTContext.IntTy;
      llvm::APInt APValue(static_cast<unsigned>(TheASTContext.getTypeSize(tip)), 1);
      cond = IntegerLiteral::Create(TheASTContext, APValue,
                                    tip, SourceLocation());
    }
    const auto petlja =
        WhileStmt::Create(TheASTContext, nullptr, cond, telo,
                          SourceLocation(), SourceLocation(), SourceLocation());

    /* Slozena naredba sa inicijalizacijom i petljom
     * ili samo petlja ako nema inicijalizacije */
    Stmt *initpet;
    if (s->getInit()) {
      initpet = CompoundStmt::Create(TheASTContext,
                    std::vector<Stmt *>{s->getInit(), petlja},
                    SourceLocation(), SourceLocation());
    } else {
      initpet = petlja;
    }

    /* Tekstualna zamena koda */
    zameni(s, initpet);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod while petlje */
bool For2WhileVisitor::TraverseForStmt(ForStmt *s) {
    return WalkUpFromForStmt(s);
}
