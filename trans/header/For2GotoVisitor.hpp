#ifndef FOR2GOTOVISITOR_HPP
#define FOR2GOTOVISITOR_HPP


#include "MTKConsumer.hpp"
#include <memory>
class For2GotoVisitor : public MTKVisitor<For2GotoVisitor> {
public:
    For2GotoVisitor(Rewriter& r, ASTContext& ac)
        : MTKVisitor(r, ac) {}


    bool TraverseFunctionDecl(FunctionDecl* );

    bool VisitBreakStmt(BreakStmt* s);

    bool VisitContinueStmt(ContinueStmt *s);

    bool VisitForStmt(ForStmt *s);
private:
    FunctionDecl *kontekstFunkcijaDecl_;
    std::string imeTrenutneFunkcije;
    struct ForLoopLabels {
        std::string nazivLabelePocetakPetlje;
        std::string nazivLabeleIncPetlje;
        std::string nazivLabeleKrajPetlje;
    };
#if 0
    static std::unordered_map<size_t, For2GotoVisitor::ForLoopLabels> forLoopGotoLabels;
    static bool zamenjeniSviBreakIContinueStmt;
    static size_t jedinstveniIdLabelePetlji_;
    static std::unordered_set<std::string> vidjeneFunkcije;
#endif

    struct FunctionData {
        size_t trenutniRedniBrojPetljeUFunkcijiZaObradu{0};
        bool zamenjeniSviBreakIContinueStmt{false};
        bool prviProlaz = true;
        std::unordered_map<size_t, For2GotoVisitor::ForLoopLabels> labeleZaPetlju{};
    };
    static std::unordered_map<std::string, std::unique_ptr<FunctionData>> podaciFunkcije;
};

#endif // FOR2GOTOVISITOR_HPP
