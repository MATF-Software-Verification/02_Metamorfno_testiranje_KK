#include "While2GotoVisitor.hpp"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/ParentMapContext.h"
/***********************
 * Shema transformacije
 * ---------------------
 * while (cond)
 *   telo;
 * ---------------------
 * loop_{loop_id}_begin:
 * if (!cond)
 *   goto loop_line_num_end:
 * telo;
 * goto loop_{loop_id}_begin;
 * loop_{loop_id}_end:
 ***********************/
#include <iostream>
#include "Assert.hpp"

std::unordered_map<size_t, While2GotoVisitor::WhileLoopLabels> While2GotoVisitor::whileLoopGotoLabels = {};
bool zamenjeniSviBreakIContinueStmt = false;
size_t While2GotoVisitor::jedinstveniIdLabelePetlji_ = 0;

bool While2GotoVisitor::VisitWhileStmt(WhileStmt *s) {

    trenutiWhile = s;
    std::cerr << __PRETTY_FUNCTION__ << '\n';
    std::cerr << id++ << '\n';
    Assert(s != nullptr, "Mora biti WhileStmt");
    Assert(kontekstFunkcijaDecl_ != nullptr, "Kontekst funkcija mora biti pronadjena pre transformacije While petlje");

    if (zamenjeniSviBreakIContinueStmt) {
        std::cerr << "Zameni WhileStmt sa GOto\n";
        auto labele = whileLoopGotoLabels.find(++jedinstveniIdLabelePetlji_);
        Assert(labele != whileLoopGotoLabels.end(), "Ne postojoje labele za dati while");

        auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.whileLoopPocetakLabelaNaziv);
        auto labelaKrajPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.whileLoopKrajLabelaNaziv);

        std::vector<Stmt*> noveNaredbe;
        noveNaredbe.reserve(5);

        noveNaredbe.emplace_back(labelaPocetakPetlje);

        auto ifCond = napraviIf(napraviNegaciju(s->getCond()), napraviGoto(labelaKrajPetlje));
        Assert(ifCond != nullptr, "Nije uspelo pravljenje if jump to end of loop");
        noveNaredbe.emplace_back(ifCond);

        noveNaredbe.emplace_back(s->getBody());

        noveNaredbe.emplace_back(napraviGoto(labelaPocetakPetlje));
        noveNaredbe.emplace_back(labelaKrajPetlje);
        zameni(s, napraviSlozenu(noveNaredbe));

        return false; // Obustavi obilzak
    } else if (whileLoopGotoLabels.find(jedinstveniIdLabelePetlji_ + 1) == std::end(whileLoopGotoLabels)){
        // Ako prvi put vidimo WhileStmt napravi jedinstevene goto labele
        // TODO(Marko): Osiguraj jedinstvenost naziva labele
        std::cerr << "Napravi GotoLabele \n";
        std::string labelaPocetakPetljeNaziv("labela_while_loop_begin");
        labelaPocetakPetljeNaziv.append(std::to_string(++jedinstveniIdLabelePetlji_));
        std::string labelaKrajPetljeNaziv("labela_while_loop_end");
        labelaKrajPetljeNaziv.append(std::to_string(jedinstveniIdLabelePetlji_));

//        auto labelaKrajPetlje =napraviLabelStmt(kontekstFunkcijaDecl_, labelaKrajPetljeNaziv);
//        Assert(labelaKrajPetlje != nullptr, "");

//        auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labelaPocetakPetljeNaziv);
//        Assert(labelaPocetakPetlje != nullptr, "");
        whileLoopGotoLabels[jedinstveniIdLabelePetlji_] = WhileLoopLabels{labelaPocetakPetljeNaziv, labelaKrajPetljeNaziv};

        return true; // Nastavi obilazak
    }

    return true;
}


bool While2GotoVisitor::VisitBreakStmt(BreakStmt *s)
{
    if (zamenjeniSviBreakIContinueStmt)
        return true;

    for (auto roditeljIter = rods.find(s); roditeljIter != std::end(rods); roditeljIter = rods.find(roditeljIter->second)) {
        auto roditelj = roditeljIter->second;
        if (const WhileStmt* whileStmtRoditelj = dyn_cast<WhileStmt>(roditelj))	{
           // Zameni break sa goto whileStmtRoditelj end loop labelom
            auto labelaKrajPetljeNaziv = whileLoopGotoLabels.find(jedinstveniIdLabelePetlji_)->second.whileLoopKrajLabelaNaziv;
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcijaDecl_, labelaKrajPetljeNaziv)));
            break;
        } else if (isa<SwitchStmt>(*roditelj) || isa<ForStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}

bool While2GotoVisitor::VisitContinueStmt(ContinueStmt *s)
{
    if (zamenjeniSviBreakIContinueStmt)
        return true;

    for (auto roditeljIter = rods.find(s); roditeljIter != std::end(rods); roditeljIter = rods.find(roditeljIter->second)) {
        auto roditelj = roditeljIter->second;
        if (const WhileStmt* whileStmtRoditelj = dyn_cast<WhileStmt>(roditelj)) {
            // Zameni continue sa goto whileStmtROiditelj begin loop labelonm
            auto labelaPocetakPetljeNaziv = whileLoopGotoLabels.find(jedinstveniIdLabelePetlji_)->second.whileLoopPocetakLabelaNaziv;
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcijaDecl_, labelaPocetakPetljeNaziv)));
            break;
        } else if (isa<ForStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}

bool While2GotoVisitor::TraverseFunctionDecl(FunctionDecl *f)
{
    std::cerr << __PRETTY_FUNCTION__ << '\n';

    kontekstFunkcijaDecl_ = f;
    if (f->hasBody()) {
        std::cerr << "\n\n";
        f->dump(llvm::errs());
        std::cerr << "\n\n";
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
