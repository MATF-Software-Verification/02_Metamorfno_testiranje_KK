#ifndef FOR2GOTOVISITOR_HPP
#define FOR2GOTOVISITOR_HPP


#include "MTKConsumer.hpp"

class For2GotoVisitor : public MTKVisitor<For2GotoVisitor> {
public:
    For2GotoVisitor(Rewriter& r, ASTContext& ac)
        : MTKVisitor(r, ac) {}


    bool TraverseFunctionDecl(FunctionDecl* );

   bool VisitForStmt(ForStmt *s);
private:
    FunctionDecl *kontekstFunkcijaDecl_;
    std::string nazivKontekstFunkcije_;

};

#endif // FOR2GOTOVISITOR_HPP
