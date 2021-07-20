#ifndef DO2GOTOVISITOR_H
#define DO2GOTOVISITOR_H

#include "MTKConsumer.hpp"
class Do2GotoVisitor : public MTKVisitor<Do2GotoVisitor>
{
public:
    Do2GotoVisitor(Rewriter& r, ASTContext& context)
        : MTKVisitor(r, context) {}
    bool VisitDoStmt(DoStmt* s);

    bool TraverseFunctionDecl(FunctionDecl* decl);
private:
    FunctionDecl *kontekstFunkcijaDecl_;
    std::string nazivKontekstFunkcije_;
};


#endif // DO2GOTOVISITOR_H
