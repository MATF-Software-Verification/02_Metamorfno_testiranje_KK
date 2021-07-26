#ifndef PREPFOR2GOTOVISITOR_HPP
#define PREPFOR2GOTOVISITOR_HPP

#include "MTKConsumer.hpp"

class PrepFor2GotoVisitor : public MTKVisitor<PrepFor2GotoVisitor> {
public:
    PrepFor2GotoVisitor(Rewriter& rw, ASTContext& ast)
        : MTKVisitor(rw, ast) {}

    bool VisitForStmt(ForStmt* s);

    bool VisitBreakStmt(BreakStmt* s);

    bool VisitContinueStmt(ContinueStmt* s);

    bool TraverseFunctionDecl(FunctionDecl* decl);

    static constexpr const char* ForLoopLabelBeginPrefixStr = "for_loop_begin_";
    static constexpr const char* ForLoopLabelIncPrefixStr = "for_loop_inc_";
    static constexpr const char* ForLoopLabelEndPrefixStr = "for_loop_end_";
private:

    FunctionDecl* kontekstFunkcija_;
    std::string nazivKontekstFunkcije_;
};

#endif // PREPFOR2GOTOVISITOR_HPP
