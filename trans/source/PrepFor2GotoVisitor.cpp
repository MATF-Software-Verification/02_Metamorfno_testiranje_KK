#include "PrepFor2GotoVisitor.hpp"
#include <unordered_map>
static size_t id = 0;
static std::unordered_map<const ForStmt*, size_t> id_petlje;

bool PrepFor2GotoVisitor::VisitForStmt(ForStmt *s)
{
    ++id;
    id_petlje[s] = id;
    return true;
}

bool PrepFor2GotoVisitor::VisitBreakStmt(BreakStmt *s)
{
     for (auto it = rods.find(s); it != rods.end(); it = rods.find(it->second)) {
        auto roditelj = it->second;
        if (const ForStmt* petlja = dyn_cast<ForStmt>(roditelj)) {
            size_t id = id_petlje.find(petlja)->second;
            std::string nazivGotoLabele(nazivKontekstFunkcije_ + "_" + ForLoopLabelEndPrefixStr);
            nazivGotoLabele.append(std::to_string(id));
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcija_, nazivGotoLabele)));
            break;
        } else if (isa<WhileStmt>(*roditelj) || isa<DoStmt>(*roditelj) || isa<SwitchStmt>(*roditelj)) {
            break;
        }
    }
    return true;

}

bool PrepFor2GotoVisitor::VisitContinueStmt(ContinueStmt *s)
{
    for (auto it = rods.find(s); it != rods.end(); it = rods.find(it->second)) {
        auto roditelj = it->second;
        if (const ForStmt* petlja = dyn_cast<ForStmt>(roditelj)) {
            size_t id = id_petlje.find(petlja)->second;
            std::string nazivGotoLabele(nazivKontekstFunkcije_ + "_" + ForLoopLabelIncPrefixStr);
            nazivGotoLabele.append(std::to_string(id));
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcija_, nazivGotoLabele)));
            break;
        } else if (isa<WhileStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}

bool PrepFor2GotoVisitor::TraverseFunctionDecl(FunctionDecl *decl)
{
    kontekstFunkcija_ = decl;
    nazivKontekstFunkcije_ = decl->getDeclName().getAsString();
    id = 0;
    id_petlje.clear();
    return MTKVisitor::TraverseFunctionDecl(decl);
}



