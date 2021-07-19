#ifndef FOR2GOTOVISITOR_HPP
#define FOR2GOTOVISITOR_HPP


#include "MTKConsumer.hpp"

class For2GotoVisitor : public MTKVisitor<For2GotoVisitor> {
public:
    For2GotoVisitor(Rewriter& r, ASTContext& ac)
        : MTKVisitor(r, ac) {}


    bool TraverseFunctionDecl(FunctionDecl* );

    bool VisitBreakStmt(BreakStmt* s);

    bool VisitContinueStmt(ContinueStmt *s);

    /* Pretvaranje for petlji u while */
    bool VisitForStmt(ForStmt *s);
private:
    FunctionDecl *kontekstFunkcijaDecl_;

    struct ForLoopLabels {
        std::string forLoopPocetakLabelaNaziv;
        std::string forLoopIncLabelaNaziv;
        std::string forLoopKrajLabelaNaziv;
    };

    static std::unordered_map<size_t, For2GotoVisitor::ForLoopLabels> forLoopGotoLabels;
    static bool zamenjeniSviBreakIContinueStmt;
    static size_t jedinstveniIdLabelePetlji_;

};

#endif // FOR2GOTOVISITOR_HPP
