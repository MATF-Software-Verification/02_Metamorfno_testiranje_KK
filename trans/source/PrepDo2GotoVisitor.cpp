#include "PrepDo2GotoVisitor.hpp"
#include <unordered_map>
static size_t id = 0;
static std::unordered_map<const DoStmt*, size_t> id_petlje;

bool PrepDo2GotoVisitor::VisitDoStmt(DoStmt *s)
{
    ++id;
    id_petlje[s] = id;
    return true;
}

bool PrepDo2GotoVisitor::VisitBreakStmt(BreakStmt *s)
{
     for (auto it = rods.find(s); it != rods.end(); it = rods.find(it->second)) {
        auto roditelj = it->second;
        if (const DoStmt* petlja = dyn_cast<DoStmt>(roditelj)) {
            size_t id = id_petlje.find(petlja)->second;
            std::string nazivGotoLabele("do_loop_end_");
            nazivGotoLabele.append(std::to_string(id));
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcija_, nazivGotoLabele)));
            break;
        } else if (isa<ForStmt>(*roditelj) || isa<WhileStmt>(*roditelj) || isa<SwitchStmt>(*roditelj)) {
            break;
        }
    }
    return true;

}

bool PrepDo2GotoVisitor::VisitContinueStmt(ContinueStmt *s)
{
    for (auto it = rods.find(s); it != rods.end(); it = rods.find(it->second)) {
        auto roditelj = it->second;
        if (const DoStmt* petlja = dyn_cast<DoStmt>(roditelj)) {
            size_t id = id_petlje.find(petlja)->second;
            std::string nazivGotoLabele("do_loop_cond_");
            nazivGotoLabele.append(std::to_string(id));
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcija_, nazivGotoLabele)));
            break;
        } else if (isa<ForStmt>(*roditelj) || isa<WhileStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}

bool PrepDo2GotoVisitor::TraverseFunctionDecl(FunctionDecl *decl)
{
    kontekstFunkcija_ = decl;
    id = 0;
    id_petlje.clear();
    return MTKVisitor::TraverseFunctionDecl(decl);
}



