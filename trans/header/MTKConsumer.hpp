#ifndef MTKCONS_H
#define MTKCONS_H

#include "MTKContext.hpp"

#include "clang/AST/ASTConsumer.h"

#include "unordered_map"

/* Pomocna natklasa sa bitnim metodama */
template <typename VisitorT>
class MTKVisitor : public MTKContext, public RecursiveASTVisitor<VisitorT> {
public:
    /* Konstruktor inicijalizuje posetioca
     * prepisivacem i kontekstom parsiranja */
    MTKVisitor(Rewriter &R, ASTContext &A)
      : MTKContext(R, A) {}

    /* Pamcenje roditelja svake naredbe */
    void izracunajDecu(Stmt *s) {
        if (!s) return;

        for (const auto dete : s->children()) {
            rods[dete] = s;
            izracunajDecu(dete);
        }
    }

    /* Javno cuvanje mape roditelja */
    std::unordered_map<const Stmt *, const Stmt *> rods;
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
        for (auto x: DR) {
            /* Izracunavanje roditeljstva */
            const auto telo = x->getBody();
            TheVisitor.rods[telo] = nullptr;
            TheVisitor.izracunajDecu(telo);

            /* Prolazak kroz deklaraciju */
            TheVisitor.TraverseDecl(x);

            /* Ciscenje roditeljstva */
            TheVisitor.rods.clear();
        }
        return true;
    }

private:
    /* Privatno cuvanje posetioca stabla */
    VisitorT TheVisitor;
};

#endif
