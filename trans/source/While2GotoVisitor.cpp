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
#include "PrepWhile2GotoVisitor.hpp"
static std::unordered_map<std::string, size_t> id_trenutne_petlje;

bool While2GotoVisitor::VisitWhileStmt(WhileStmt *s) {

    size_t id = ++id_trenutne_petlje[nazivKontekstFunkcije_];

    std::string nazivLabelePocetakPetlje(nazivKontekstFunkcije_ + "_" + PrepWhile2GotoVisitor::WhileLoopBeginLabelPrefixStr);
    nazivLabelePocetakPetlje.append(std::to_string(id));

    std::string nazivLabeleKrajPetlje(nazivKontekstFunkcije_ + "_" +PrepWhile2GotoVisitor::WhileLoopEndLabelPrefixStr);
    nazivLabeleKrajPetlje.append(std::to_string(id));

    auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcija_, nazivLabelePocetakPetlje);
    auto labelaKrajPetlje = napraviLabelStmt(kontekstFunkcija_, nazivLabeleKrajPetlje);


    auto ifCond = napraviIf(napraviNegaciju(s->getCond()), napraviGoto(labelaKrajPetlje));
    auto teloPetlje = s->getBody();


    std::vector<Stmt*> noveNaredbe;
    noveNaredbe.emplace_back(labelaPocetakPetlje);
    noveNaredbe.emplace_back(ifCond);
    noveNaredbe.emplace_back(teloPetlje);
    noveNaredbe.emplace_back(napraviGoto(labelaPocetakPetlje)); // <-- u refaktorisanju sam ovo zaboravio!
    noveNaredbe.emplace_back(labelaKrajPetlje);

    zameni(s, napraviSlozenu(noveNaredbe));
    return false;
}

bool While2GotoVisitor::TraverseFunctionDecl(FunctionDecl *f)
{
    kontekstFunkcija_ = f;
    nazivKontekstFunkcije_ = f->getDeclName().getAsString();
    return MTKVisitor::TraverseFunctionDecl(f);
}
