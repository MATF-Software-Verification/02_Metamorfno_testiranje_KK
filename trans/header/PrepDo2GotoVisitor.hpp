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

private:

    FunctionDecl* kontekstFunkcija_;
};

#endif // PREPDO2GOTO_HPP
