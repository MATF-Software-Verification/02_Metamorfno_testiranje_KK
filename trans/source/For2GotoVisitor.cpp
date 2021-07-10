#include "For2GotoVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * for (init; cond; inc)
 *   telo;
 * ---------------------
 * init;
 * for_loop_begin_{unique_id}:
 * if (!cond)
 *    goto for_loop_end_{unique_id};
 * telo;
 * inc;
 * goto for_loop_begin_unique_id;
 * for_loop_end_{unique_id};
 ***********************/

/* Posetilac koji for pretvara u while */
bool For2GotoVisitor::VisitFunctionDecl(FunctionDecl *f)
{
   kontekstFunkcija_ = f;
   return true;
}

bool For2GotoVisitor::VisitForStmt(ForStmt *s) {
    std::vector<Stmt*> naredbe;
    naredbe.reserve(7);

    {
        auto init = s->getInit();
        naredbe.push_back(init);
    }

    std::string labelaForLoopBeginNaziv("for_loop_begin_");
    labelaForLoopBeginNaziv.append(std::to_string(jedinstveniIdLabela_++));
    auto forLoopBeginLabel = napraviLabelStmt(kontekstFunkcija_, labelaForLoopBeginNaziv);

    std::string labelaForLoopEndNaziv("for_loop_end_");
    labelaForLoopEndNaziv.append(std::to_string(jedinstveniIdLabela_++));

    auto forLoopEndLabel = napraviLabelStmt(kontekstFunkcija_, labelaForLoopEndNaziv);

    {
        auto gotoStmtLoopEnd = napraviGoto(forLoopEndLabel);
        auto ifCond = napraviIf(s->getCond(), gotoStmtLoopEnd);
        naredbe.push_back(ifCond);
        naredbe.push_back(s->getBody());
        naredbe.push_back(s->getInc());
    }

    {
        auto gotoStmtLoopBegin = napraviGoto(forLoopBeginLabel);
        naredbe.push_back(gotoStmtLoopBegin);
        naredbe.push_back(forLoopEndLabel);
    }
    auto slozeneNaredbe = napraviSlozenu(naredbe);

    zameni(s, slozeneNaredbe);

    return true;
}

/* Prekid obilaska kod while petlje */
bool For2GotoVisitor::TraverseForStmt(ForStmt *s) {
    return WalkUpFromForStmt(s);
}
