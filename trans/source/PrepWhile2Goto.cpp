#include "PrepWhile2Goto.hpp"
#include <unordered_map>
static size_t id = 0;
static std::unordered_map<const WhileStmt*, size_t> id_petlje;

bool PrepWhile2GotoVisitor::VisitWhileStmt(WhileStmt *s)
{
    ++id;
    id_petlje[s] = id;
    return true;
}

bool PrepWhile2GotoVisitor::VisitBreakStmt(BreakStmt *s)
{
     for (auto it = rods.find(s); it != rods.end(); it = rods.find(it->second)) {
        auto roditelj = it->second;
        if (const WhileStmt* petlja = dyn_cast<WhileStmt>(roditelj)) {
            size_t id = id_petlje.find(petlja)->second;
            std::string nazivGotoLabele("while_loop_end_");
            nazivGotoLabele.append(std::to_string(id));
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcija_, nazivGotoLabele)));
            break;
        } else if (isa<ForStmt>(*roditelj) || isa<DoStmt>(*roditelj) || isa<SwitchStmt>(*roditelj)) {
            break;
        }
    }
    return true;

}

bool PrepWhile2GotoVisitor::VisitContinueStmt(ContinueStmt *s)
{
    for (auto it = rods.find(s); it != rods.end(); it = rods.find(it->second)) {
        auto roditelj = it->second;
        if (const WhileStmt* petlja = dyn_cast<WhileStmt>(roditelj)) {
            size_t id = id_petlje.find(petlja)->second;
            std::string nazivGotoLabele("while_loop_begin_");
            nazivGotoLabele.append(std::to_string(id));
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcija_, nazivGotoLabele)));
            break;
        } else if (isa<ForStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}

bool PrepWhile2GotoVisitor::TraverseFunctionDecl(FunctionDecl *decl)
{
    kontekstFunkcija_ = decl;
    id = 0;
    id_petlje.clear();
    return MTKVisitor::TraverseFunctionDecl(decl);
}



