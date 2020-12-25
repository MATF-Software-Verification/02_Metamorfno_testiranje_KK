#ifndef CODEIMPUT_H
#define CODEIMPUT_H

#include "MTKConsumer.hpp"

/* Posetilac koji umece kod */
class CodeImputVisitor : public MTKVisitor<CodeImputVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    CodeImputVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) {}

    /* Postavljanje broja umetanja */
    static void postaviBroj(unsigned long long i);

private:
    /* Staticki podatak o broju umetanja */
    static unsigned long long n;
};

#endif
