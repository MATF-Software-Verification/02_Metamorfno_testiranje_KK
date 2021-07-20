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

private:

    FunctionDecl* kontekstFunkcija_;
};

#endif // PREPWHILE2GOTOVISITOR_HPP
