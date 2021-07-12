#include "For2GotoVisitor.hpp"
#include "Assert.hpp"

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
    Assert(s != nullptr, "");
    Assert(kontekstFunkcija_ != nullptr, "");
    std::vector<Stmt*> naredbe;
    naredbe.reserve(7);
    Assert(naredbe.capacity() >= 7, "");
    {
        auto init = s->getInit();
        Assert(init != nullptr, "");
        naredbe.push_back(init);
    }

    std::string labelaForLoopBeginNaziv("for_loop_begin_");
    labelaForLoopBeginNaziv.append(std::to_string(jedinstveniIdLabela_++));
    auto forLoopBeginLabel = napraviLabelStmt(kontekstFunkcija_, labelaForLoopBeginNaziv);
    Assert(forLoopBeginLabel != nullptr, "");

    std::string labelaForLoopEndNaziv("for_loop_end_");
    labelaForLoopEndNaziv.append(std::to_string(jedinstveniIdLabela_++));

    auto forLoopEndLabel = napraviLabelStmt(kontekstFunkcija_, labelaForLoopEndNaziv);
    Assert(forLoopEndLabel != nullptr, "");
    {
        auto gotoStmtLoopEnd = napraviGoto(forLoopEndLabel);
        Assert(gotoStmtLoopEnd != nullptr, "");
        auto ifCond = napraviIf(s->getCond(), gotoStmtLoopEnd);
        Assert(ifCond != nullptr, "");
        naredbe.push_back(ifCond);
        naredbe.push_back(s->getBody());
        naredbe.push_back(s->getInc());
    }

    {
        auto gotoStmtLoopBegin = napraviGoto(forLoopBeginLabel);
        Assert(gotoStmtLoopBegin != nullptr, "");
        naredbe.push_back(gotoStmtLoopBegin);
        naredbe.push_back(forLoopEndLabel);
    }
    auto slozeneNaredbe = napraviSlozenu(naredbe);
    Assert(slozeneNaredbe != nullptr, "");
    zameni(s, slozeneNaredbe);

    return true;
}

/* Prekid obilaska kod while petlje */
bool For2GotoVisitor::TraverseForStmt(ForStmt *s) {
    return WalkUpFromForStmt(s);
}
