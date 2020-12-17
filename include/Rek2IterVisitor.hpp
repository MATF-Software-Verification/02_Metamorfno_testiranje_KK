#ifndef REK2ITER_H
#define REK2ITER_H

#include "MTKConsumer.hpp"

/* Posetilac koji dodaje korak petlje */
class Rek2IterVisitor : public MTKVisitor<Rek2IterVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    Rek2IterVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}
};

#endif
