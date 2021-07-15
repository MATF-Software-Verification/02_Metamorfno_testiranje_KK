#ifndef FOR2GOTOVISITOR_HPP
#define FOR2GOTOVISITOR_HPP


#include "MTKConsumer.hpp"

class For2GotoVisitor : public MTKVisitor<For2GotoVisitor> {
public:
    For2GotoVisitor(Rewriter& r, ASTContext& ac)
        : MTKVisitor(r, ac) {}


    bool TraverseFunctionDecl(FunctionDecl* );

    /* Pretvaranje for petlji u while */
    bool VisitForStmt(ForStmt *s);

    /* Prekid obilaska kod for petlje */
    bool TraverseForStmt(ForStmt *s);
private:
    FunctionDecl *kontekstFunkcija_;
    size_t jedinstveniIdLabela_{0};
};

#endif // FOR2GOTOVISITOR_HPP
