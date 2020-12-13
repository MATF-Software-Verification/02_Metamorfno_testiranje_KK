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

    /* Pronalazak prvog slobodnog imena */
    std::string nadjiIme(const std::string &pocetno) const {
        /* Pomocne promenljive za pretragu */
        std::string ime(pocetno);
        auto i = 0ull; /* brojac */

        /* Proba svih mogucih kombinacija */
        while (TheASTContext.Idents.find(ime)
               != TheASTContext.Idents.end()) {
            if (i == ULLONG_MAX) exit(EXIT_FAILURE);
            ime = pocetno + std::to_string(i++);
        }

        /* Vracanje pronadjenog imena */
        return ime;
    }

    /* Pravljenje nove deklaracije */
    VarDecl *napraviDecl(DeclContext *kontekst,
                         const CanQual<Type> &tip,
                         const std::string &ime) {
        return VarDecl::Create(TheASTContext, kontekst,
                               SourceLocation(), SourceLocation(),
                               &TheASTContext.Idents.getOwn(ime),
                               tip, nullptr, SC_None);
    }

    /* Pravljenje izraza deklaracije */
    DeclRefExpr *napraviDeclExpr(ValueDecl *deklaracija,
                                 const CanQual<Type> &tip) {
        return DeclRefExpr::Create(TheASTContext, NestedNameSpecifierLoc(),
                                   SourceLocation(), deklaracija, true,
                                   SourceLocation(), tip, VK_LValue);
    }

    /* Pravljenje celobrojne vrednosti */
    IntegerLiteral *napraviInt(int val) {
        const auto tip = TheASTContext.IntTy;
        llvm::APInt APValue(static_cast<unsigned>(TheASTContext.getTypeSize(tip)), val);
        return IntegerLiteral::Create(TheASTContext, APValue,
                                      tip, SourceLocation());
    }

    /* Pravljenje tacne istinitosne vrednosti */
    IntegerLiteral *napraviTrue() {
        return napraviInt(1);
    }

    /* Pravljenje nrtacne istinitosne vrednosti */
    IntegerLiteral *napraviFalse() {
        return napraviInt(0);
    }

    /* Pravljenje binarnog operatora */
    BinaryOperator *napraviBinarni(Expr *lhs, Expr *rhs,
                                   const BinaryOperator::Opcode &op,
                                   const CanQual<Type> &tip) {
        return BinaryOperator::Create(TheASTContext, lhs, rhs, op,
                                      tip, VK_RValue, OK_Ordinary,
                                      SourceLocation(), FPOptionsOverride());
    }

    /* Pravljenje izraza dodele */
    BinaryOperator *napraviDodelu(Expr *lhs, Expr *rhs,
                                  const CanQual<Type> &tip) {
        return napraviBinarni(lhs, rhs, BO_Assign, tip);
    }

    /* Pravljenje slozene naredbe */
    CompoundStmt *napraviSlozenu(const std::vector<Stmt *> &naredbe) {
        return CompoundStmt::Create(TheASTContext, naredbe,
                                    SourceLocation(), SourceLocation());
    }

    /* Pravljenje uslovne naredbe */
    IfStmt *napraviIf(Expr *ako, Stmt *onda, Stmt *inace = nullptr) {
        return IfStmt::Create(TheASTContext, SourceLocation(),
                              false, nullptr, nullptr, ako,
                              onda, SourceLocation(), inace);
    }

    /* Pravljenje do petlje */
    DoStmt *napraviDo(Stmt *telo, Expr *uslov) {
        DoStmt petlja(telo, uslov, SourceLocation(),
                      SourceLocation(), SourceLocation());
        auto *adresa = static_cast<DoStmt *>(malloc(sizeof(petlja)));
        memcpy(adresa, &petlja, sizeof(petlja));
        return adresa;
    }

    /* Pravljenje while petlje */
    WhileStmt *napraviWhile(Expr *uslov, Stmt *telo) {
        return WhileStmt::Create(TheASTContext, nullptr, uslov, telo,
                                 SourceLocation(), SourceLocation(), SourceLocation());
    }

    /* Pravljenje for petlje */
    ForStmt *napraviFor(Expr *uslov, Expr *korak, Stmt *telo) {
        ForStmt petlja(TheASTContext, nullptr, uslov, nullptr, korak, telo,
                       SourceLocation(), SourceLocation(), SourceLocation());
        auto *adresa = static_cast<ForStmt *>(malloc(sizeof(petlja)));
        memcpy(adresa, &petlja, sizeof(petlja));
        return adresa;
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
