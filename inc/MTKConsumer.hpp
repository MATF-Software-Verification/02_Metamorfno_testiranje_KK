#ifndef MTKVISITOR_H
#define MTKVISITOR_H

/* Ukljucivanje standardnih biblioteka */
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>
#include <system_error>

/* Ukljucivanje Clangovih biblioteka */
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

/* Upotreba Clangovog imenskog prostora */
using namespace clang;

/* Pomocna natklasa sa bitnim metodama */
template <typename VisitorT>
class MTKVisitor : public RecursiveASTVisitor<VisitorT> {
public:
    /* Konstruktor inicijalizuje posetioca
     * prepisivacem i kontekstom parsiranja */
    MTKVisitor(Rewriter &R, ASTContext &A)
      : TheRewriter(R), TheASTContext(A) {}

    /* Tekstualna reprezentacija naredbe */
    std::string stampaj(const Stmt *const s) const {
        /* Inicijalizacija izlaznog toka */
       std::string stmt;
       llvm::raw_string_ostream stream(stmt);

        /* Stampanje naredbe u tok */
        s->printPretty(stream, nullptr, PrintingPolicy(LangOptions()));
        stream.flush();

        /* Uklanjanje suvisnog novog reda */
        stmt.pop_back();

        /* Vracanje rezultata */
        return stmt;
    }

    /* Tekstualna zamena koda */
    void zameni(const Stmt *const stari, const Stmt *const novi) {
        /* Tekstualna reprezentacija novog iskaza */
        const auto stmt = stampaj(novi);

        /* Granicne oznake u kodu */
        const auto start = stari->getSourceRange().getBegin();
        const auto end = stari->getSourceRange().getEnd();

        /* Dohvatanje poslednjeg tokena */
        Token tok;
        Lexer::getRawToken(end, tok, TheRewriter.getSourceMgr(),
                           TheRewriter.getLangOpts());
        const std::string ime = tok.getName();

        /* Racunanje offseta osim ukoliko je kraj slozene naredbe;
         * tada je tacka-zapeta ili zatvorena zagrada suvisni token */
        const auto offset = Lexer::MeasureTokenLength(end,
                                TheRewriter.getSourceMgr(),
                                TheRewriter.getLangOpts())
                                + (ime != "r_brace" && ime != "semi");

        /* Promena teksta na izracunatom mestu */
        const SourceRange sr(start, end.getLocWithOffset(offset));
        TheRewriter.ReplaceText(sr, stmt);
    }

protected:
    /* Zasticeno cuvanje prepisivaca i konteksta */
    Rewriter &TheRewriter;
    ASTContext &TheASTContext;
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
