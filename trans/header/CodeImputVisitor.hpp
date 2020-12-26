#ifndef CODEIMPUT_H
#define CODEIMPUT_H

#include "MTKConsumer.hpp"

/* Posetilac koji umece kod */
class CodeImputVisitor : public MTKVisitor<CodeImputVisitor> {
public:
    /* Inicijalizacija prepisivaca i konteksta */
    CodeImputVisitor(Rewriter &R, ASTContext &A)
      : MTKVisitor(R, A) { srand(0); }

    /* Postavljanje broja umetanja */
    static void postaviBroj(unsigned long long i);

    /* Imputacija oko naredbe ako treba */
    bool VisitStmt(Stmt *s);

    /* Provera da li je moguca izmena */
    bool nemogucaIzmena(Stmt *s) const;

    /* Obilazak svih naredbi */
    bool TraverseStmt(Stmt *s);

private:
    /* Skup naredbi koje se ne menjaju */
    std::unordered_set<Stmt *> tabu;

    /* Staticki podatak o verovatnoci umetanja */
    static unsigned long long n;

    /* Staticki spisak datoteka za dodavanje */
    static const char *dats[];

    /* Informacija o mogucoj gresci */
    static constexpr auto nemaDatoteke = "Neuspelo otvaranje datoteke!";
};

#endif
