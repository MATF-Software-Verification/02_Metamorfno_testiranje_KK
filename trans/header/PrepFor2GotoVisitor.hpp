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

private:

    FunctionDecl* kontekstFunkcija_;
};

#endif // PREPFOR2GOTOVISITOR_HPP
