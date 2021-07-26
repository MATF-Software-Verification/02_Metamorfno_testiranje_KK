#ifndef PREPWHILE2GOTOVISITOR_HPP
#define PREPWHILE2GOTOVISITOR_HPP

#include "MTKConsumer.hpp"

class PrepWhile2GotoVisitor : public MTKVisitor<PrepWhile2GotoVisitor> {
public:
    PrepWhile2GotoVisitor(Rewriter& rw, ASTContext& ast)
        : MTKVisitor(rw, ast) {}

    bool VisitWhileStmt(WhileStmt* s);

    bool VisitBreakStmt(BreakStmt* s);

    bool VisitContinueStmt(ContinueStmt* s);

    bool TraverseFunctionDecl(FunctionDecl* decl);

    static constexpr const char* WhileLoopBeginLabelPrefixStr = "while_loop_begin_";
    static constexpr const char* WhileLoopEndLabelPrefixStr = "while_loop_end_";

private:

    FunctionDecl* kontekstFunkcija_;
    std::string nazivKontekstFunkcije_;
};

#endif // PREPWHILE2GOTOVISITOR_HPP
