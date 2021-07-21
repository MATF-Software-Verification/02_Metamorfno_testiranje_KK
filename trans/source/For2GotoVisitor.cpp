#include "For2GotoVisitor.hpp"
#include "Assert.hpp"
#include "PrepFor2GotoVisitor.hpp"
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
#include<iostream>

static std::unordered_map<std::string, size_t> id_trenutne_petlje;
/* Posetilac koji for pretvara u while */
bool For2GotoVisitor::TraverseFunctionDecl(FunctionDecl *f)
{
    kontekstFunkcijaDecl_ = f;
    nazivKontekstFunkcije_ = f->getDeclName().getAsString();
    return MTKVisitor::TraverseFunctionDecl(f);
}

bool For2GotoVisitor::VisitForStmt(ForStmt *s)
{
    size_t id = ++id_trenutne_petlje[nazivKontekstFunkcije_];

    std::string nazivLabelePocetak(PrepFor2GotoVisitor::ForLoopLabelBeginPrefixStr);
    nazivLabelePocetak.append(std::to_string(id));

    std::string nazivLabeleKraj(PrepFor2GotoVisitor::ForLoopLabelEndPrefixStr);
    nazivLabeleKraj.append(std::to_string(id));

    std::string nazivLabeleInc(PrepFor2GotoVisitor::ForLoopLabelIncPrefixStr);
    nazivLabeleInc.append(std::to_string(id));


    auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, nazivLabelePocetak);
    auto labelaKrajPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, nazivLabeleKraj);
    auto labelaIncPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, nazivLabeleInc);

    std::vector<Stmt*> noveNaredbe;
    if (auto initPetlje = s->getInit()) {
        noveNaredbe.emplace_back(initPetlje);
    }

    noveNaredbe.emplace_back(labelaPocetakPetlje);
    if (auto condPetlje = s->getCond()) {
        auto ifCond = napraviIf(napraviNegaciju(condPetlje), napraviGoto(labelaKrajPetlje));
        noveNaredbe.emplace_back(ifCond);
    }

    auto teloPetlje = s->getBody();
    noveNaredbe.emplace_back(teloPetlje);

    noveNaredbe.emplace_back(labelaIncPetlje);
    if (auto incPetlje = s->getInc()) {
        noveNaredbe.emplace_back(incPetlje);
    }
    noveNaredbe.emplace_back(napraviGoto(labelaPocetakPetlje));
    noveNaredbe.emplace_back(labelaKrajPetlje);
    zameni(s, napraviSlozenu(noveNaredbe));
    return false;
}


