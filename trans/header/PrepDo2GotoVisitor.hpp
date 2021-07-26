#ifndef PREPDO2GOTO_HPP
#define PREPDO2GOTO_HPP

#include "MTKConsumer.hpp"

class PrepDo2GotoVisitor : public MTKVisitor<PrepDo2GotoVisitor> {
public:
    PrepDo2GotoVisitor(Rewriter& rw, ASTContext& ast)
        : MTKVisitor(rw, ast) {}

    bool VisitDoStmt(DoStmt* s);

    bool VisitBreakStmt(BreakStmt* s);

    bool VisitContinueStmt(ContinueStmt* s);

    bool TraverseFunctionDecl(FunctionDecl* decl);

    static constexpr const char* DoLoopBeginLabelPrefixStr = "do_loop_begin_";
    static constexpr const char* DoLoopCondLabelPrefixStr = "do_loop_cond_";
    static constexpr const char* DoLoopEndLabelPrefixStr = "do_loop_end_";
private:

    FunctionDecl* kontekstFunkcija_;
    std::string nazivKontekstFunkcije_;
};

#endif // PREPDO2GOTO_HPP
