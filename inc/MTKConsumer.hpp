#ifndef MTKVISITOR_H
#define MTKVISITOR_H

#include "MTKContext.hpp"

#include "clang/AST/ASTConsumer.h"

/* Pomocna natklasa sa bitnim metodama */
template <typename VisitorT>
class MTKVisitor : public MTKContext, public RecursiveASTVisitor<VisitorT> {
public:
    /* Konstruktor inicijalizuje posetioca
     * prepisivacem i kontekstom parsiranja */
    MTKVisitor(Rewriter &R, ASTContext &A)
      : MTKContext(R, A) {}
};

/* Klasa za obradu dobijenog AST stabla */
template <typename VisitorT>
class MTKConsumer : public ASTConsumer {
public:
    /* Konstruktor inicijalizuje posetioca
     * prepisivacem i kontekstom parsiranja */
    MTKConsumer(Rewriter &R, ASTContext &A)
      : TheVisitor(R, A) {}

    /* Svaka deklaracija obradjuje se zasebno */
    bool HandleTopLevelDecl(DeclGroupRef DR) override {
        for (auto x: DR)
            TheVisitor.TraverseDecl(x);
        return true;
    }

private:
    /* Privatno cuvanje posetioca stabla */
    VisitorT TheVisitor;
};

#endif
