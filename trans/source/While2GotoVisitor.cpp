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

#include "Assert.hpp"
bool While2GotoVisitor::VisitWhileStmt(WhileStmt *s) {
    Assert(s != nullptr, "Mora biti WhileStmt");
    Assert(kontekstFunkcijaDecl_ != nullptr, "Kontekst funkcija mora biti pronadjena pre transformacije While petlje");
    std::vector<Stmt*> naredbe;
    naredbe.reserve(5);

    // TODO(Marko): Osiguraj jedinstvenost naziva labele
    std::string labelaPocetakPetljeNaziv("labela_while_loop_begin");
    labelaPocetakPetljeNaziv.append(std::to_string(jedinstveniIdLabelePetlji_++));
    std::string labelaKrajPetljeNaziv("labela_while_loop_end");
    labelaKrajPetljeNaziv.append(std::to_string(jedinstveniIdLabelePetlji_++));

    auto labelaKrajPetlje =napraviLabelStmt(kontekstFunkcijaDecl_, labelaKrajPetljeNaziv);
    Assert(labelaKrajPetlje != nullptr, "");

    auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labelaPocetakPetljeNaziv);
    Assert(labelaPocetakPetlje != nullptr, "");
    naredbe.emplace_back(labelaPocetakPetlje);

    auto ifCond = napraviIf(s->getCond(), napraviGoto(labelaKrajPetlje));
    Assert(ifCond != nullptr, "");
    naredbe.emplace_back(ifCond);

    auto teloWhile = s->getBody();
    Assert(teloWhile != nullptr, "");
    naredbe.emplace_back(teloWhile);

    auto gotoLabelaPocetakPetlje = napraviGoto(labelaPocetakPetlje);
    Assert(gotoLabelaPocetakPetlje != nullptr, "");
    naredbe.emplace_back(gotoLabelaPocetakPetlje);

    naredbe.emplace_back(labelaKrajPetlje);

    auto slozenaNaredba = napraviSlozenu(naredbe);
    Assert(slozenaNaredba != nullptr, "");
    zameni(s, slozenaNaredba);
    return true;
}

bool While2GotoVisitor::TraverseWhileStmt(WhileStmt *s) {
    return WalkUpFromWhileStmt(s);
}

bool While2GotoVisitor::TraverseFunctionDecl(FunctionDecl *f)
{
    kontekstFunkcijaDecl_ = f;
    return true;
}
