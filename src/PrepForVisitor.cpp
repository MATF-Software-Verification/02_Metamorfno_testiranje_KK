#include "PrepForVisitor.hpp"

#include "clang/AST/ParentMapContext.h"

/* Dodavanje koraka for petlje pre continue */
bool PrepForVisitor::VisitContinueStmt(ContinueStmt *s) {
    /* Inicijalizacija nove naredbe */
    Stmt *stmt = s;

    /* Prolazak kroz roditelje tekuceg continue */
    auto rod = TheASTContext.getParentMapContext().getParents(*s);
    while (!rod.empty()) {
        /* Izdvajanje glavnog roditelja */
        const auto *r = rod.begin()->get<Stmt>();

        /* Odustajanje ako je do ili while */
        if (isa<DoStmt>(r) || isa<WhileStmt>(r))
            return true;

        /* Uzimanje roditelja koji je for */
        if (auto *rr = dyn_cast<ForStmt>(r)) {
            /* Odustajanje ako nema inkrement */
            if (!rr->getInc())
                return true;

            /* Pravljenje nove naredbe */
            auto inc = const_cast<Stmt *>(cast<Stmt>(rr->getInc()));
            stmt = CompoundStmt::Create(TheASTContext,
                       std::vector<Stmt *>{inc, s},
                       SourceLocation(), SourceLocation());
            break;
        }

        /* Nastavljanje dalje */
        rod = TheASTContext.getParents(*r);
    }

    /* Tekstualna zamena koda */
    zameni(s, stmt);

    /* Nastavljanje dalje */
    return true;
}
