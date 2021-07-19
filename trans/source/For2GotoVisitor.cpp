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

std::unordered_map<size_t, For2GotoVisitor::ForLoopLabels> For2GotoVisitor::forLoopGotoLabels = {};
bool For2GotoVisitor::zamenjeniSviBreakIContinueStmt = false;
size_t For2GotoVisitor::jedinstveniIdLabelePetlji_ = 0;

/* Posetilac koji for pretvara u while */
bool For2GotoVisitor::TraverseFunctionDecl(FunctionDecl *f)
{
      kontekstFunkcijaDecl_ = f;
    if (f->hasBody()) {
        MTKVisitor::izracunajDecu(f->getBody());
    }

    bool rezultat = MTKVisitor::TraverseFunctionDecl(f);
    if (zamenjeniSviBreakIContinueStmt == false) {
        jedinstveniIdLabelePetlji_ = 0;
    }
    zamenjeniSviBreakIContinueStmt = true; // kada se prvi put prodje kroz ast sve break i continue naredbe su zamenjenje
    // kada se visitor pozove drugi put pocece da menja while petlje
    return rezultat;
}

bool For2GotoVisitor::VisitForStmt(ForStmt *s) {
    Assert(s != nullptr, "nema ForStmt");

    if (zamenjeniSviBreakIContinueStmt) {
        // Zameni for petlju u goto
        std::vector<Stmt*> noveNaredbe;
        if (auto initStmt = s->getInit()) {
            noveNaredbe.emplace_back(initStmt);
        }
        auto labele = forLoopGotoLabels.find(++jedinstveniIdLabelePetlji_);
        Assert(labele != forLoopGotoLabels.end(), "Nema labeli");
        auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.forLoopPocetakLabelaNaziv);
        auto labelaKrajPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.forLoopKrajLabelaNaziv);

        noveNaredbe.emplace_back(labelaPocetakPetlje);

        if (auto condExpr = s->getCond()) {
            noveNaredbe.emplace_back(napraviIf(napraviNegaciju(condExpr), napraviGoto(labelaKrajPetlje)));
        }

        if (auto loopBody = s->getBody()) {
            noveNaredbe.emplace_back(napraviSlozenu({loopBody}));
        } else {
            noveNaredbe.emplace_back(napraviNullStmt());
        }

        noveNaredbe.emplace_back(napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.forLoopIncLabelaNaziv));
        if (auto loopIncr = s->getInc()) {
            noveNaredbe.emplace_back(loopIncr);
        }
        noveNaredbe.emplace_back(napraviGoto(labelaPocetakPetlje));
        noveNaredbe.emplace_back(labelaKrajPetlje);
        zameni(s, napraviSlozenu(noveNaredbe));
        return false; // zaustavi oblizak
    } else if (forLoopGotoLabels.find(jedinstveniIdLabelePetlji_ + 1) == forLoopGotoLabels.end()) {
        // Prvi put
        // Ako prvi put vidimo ForStmt napravi jedinstevene goto labele
        // TODO(Marko): Osiguraj jedinstvenost naziva labele
        std::string labelaPocetakPetljeNaziv("labela_for_loop_begin_");
        labelaPocetakPetljeNaziv.append(std::to_string(++jedinstveniIdLabelePetlji_));
        std::string labelaKrajPetljeNaziv("labela_for_loop_end_");
        labelaKrajPetljeNaziv.append(std::to_string(jedinstveniIdLabelePetlji_));

        std::string labelaIncPetljeNaziv("labela_for_loop_inc_");
        labelaIncPetljeNaziv.append(std::to_string(jedinstveniIdLabelePetlji_));

        forLoopGotoLabels[jedinstveniIdLabelePetlji_] = ForLoopLabels{labelaPocetakPetljeNaziv, labelaIncPetljeNaziv, labelaKrajPetljeNaziv};
        return true; // nastavi oblizak

    }
    return true;
}


bool For2GotoVisitor::VisitBreakStmt(BreakStmt *s)
{
    if (zamenjeniSviBreakIContinueStmt)
        return true;

    for (auto roditeljIter = rods.find(s); roditeljIter != std::end(rods); roditeljIter = rods.find(roditeljIter->second)) {
        auto roditelj = roditeljIter->second;
        if (const ForStmt* forStmtRoditelj = dyn_cast<ForStmt>(roditelj))	{
           // Zameni break sa goto whileStmtRoditelj end loop labelom
            auto labelaKrajPetljeNaziv = forLoopGotoLabels.find(jedinstveniIdLabelePetlji_)->second.forLoopKrajLabelaNaziv;
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcijaDecl_, labelaKrajPetljeNaziv)));
            break;
        } else if (isa<SwitchStmt>(*roditelj) || isa<WhileStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}

bool For2GotoVisitor::VisitContinueStmt(ContinueStmt *s)
{
    if (zamenjeniSviBreakIContinueStmt)
        return true;

    for (auto roditeljIter = rods.find(s); roditeljIter != std::end(rods); roditeljIter = rods.find(roditeljIter->second)) {
        auto roditelj = roditeljIter->second;
        if (const ForStmt* forStmtRoditelj = dyn_cast<ForStmt>(roditelj)) {
            // Zameni continue sa goto whileStmtROiditelj begin loop labelonm
            auto labelaIncPetljeNaziv = forLoopGotoLabels.find(jedinstveniIdLabelePetlji_)->second.forLoopIncLabelaNaziv;
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcijaDecl_, labelaIncPetljeNaziv)));
            break;
        } else if (isa<WhileStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}



