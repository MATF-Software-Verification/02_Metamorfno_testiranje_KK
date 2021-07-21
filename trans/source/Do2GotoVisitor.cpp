#include "Do2GotoVisitor.h"
#include "Assert.hpp"
#include "PrepDo2GotoVisitor.hpp"
#include<unordered_map>
static std::unordered_map<const DoStmt*, size_t> id_petlje_trenutne_petlje;

/* Posetilac koji for pretvara u while */
bool Do2GotoVisitor::TraverseFunctionDecl(FunctionDecl *f)
{
    kontekstFunkcijaDecl_ = f;
    nazivKontekstFunkcije_ = f->getDeclName().getAsString();
    return MTKVisitor::TraverseFunctionDecl(f);

}

bool Do2GotoVisitor::VisitDoStmt(DoStmt *s) {

    size_t id = ++id_petlje_trenutne_petlje[s];

    std::string nazivLabelePocetakPetlje(PrepDo2GotoVisitor::DoLoopBeginLabelPrefixStr);
    nazivLabelePocetakPetlje.append(std::to_string(id));

    std::string nazivLabeleCondPetlje(PrepDo2GotoVisitor::DoLoopCondLabelPrefixStr);
    nazivLabeleCondPetlje.append(std::to_string(id));

    std::string nazivLabeleKrajPetlje(PrepDo2GotoVisitor::DoLoopEndLabelPrefixStr);
    nazivLabeleKrajPetlje.append(std::to_string(id));

    auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, nazivLabelePocetakPetlje);
    auto labelaKrajPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, nazivLabeleKrajPetlje);
    auto labelaCondPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, nazivLabeleCondPetlje);

    auto condPetlje = napraviIf(s->getCond(), napraviGoto(labelaPocetakPetlje));

    std::vector<Stmt*> noveNaredbe;
    noveNaredbe.emplace_back(labelaPocetakPetlje);
    noveNaredbe.emplace_back(s->getBody());
    noveNaredbe.emplace_back(labelaCondPetlje);
    noveNaredbe.emplace_back(condPetlje);
    noveNaredbe.emplace_back(labelaKrajPetlje);

    zameni(s, napraviSlozenu(noveNaredbe));
    return false;
}



