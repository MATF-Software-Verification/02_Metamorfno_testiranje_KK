#ifndef MTKCONTEXT_H
#define MTKCONTEXT_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"

/* Upotreba Clangovog imenskog prostora */
using namespace clang;

/* Pomocna natklasa sa bitnim metodama */
class MTKContext {
public:
    /* Konstruktor inicijalizuje posetioca
     * prepisivacem i kontekstom parsiranja */
    MTKContext(Rewriter &R, ASTContext &A)
      : TheRewriter(R), TheASTContext(A) {}

    /* Tekstualna reprezentacija naredbe */
    std::string stampaj(const Stmt *const s) const;

    /* Odredjivanje mesta naredbe u kodu */
    SourceRange odrediMesto(const Stmt *const s) const;

    /* Tekstualna zamena koda */
    void zameni(const Stmt *const stari, const Stmt *const novi) const;

    /* Prednja tekstualna dopuna koda */
    void dodajIspred(const Stmt *const stari, const Stmt *const novi) const;

    /* Zadnja tekstualna dopuna koda */
    void dodajIza(const Stmt *const stari, const Stmt *const novi) const;

    /* Pronalazak prvog slobodnog imena */
    std::string nadjiIme(const std::string &pocetno) const;

    /* Pravljenje nove deklaracije */
    VarDecl *napraviDecl(DeclContext *kontekst,
                         const CanQual<Type> &tip,
                         const std::string &ime) const;

    /* Pravljenje izraza deklaracije */
    DeclRefExpr *napraviDeclExpr(DeclStmt *deknar) const;

    /* Pravljenje celobrojne vrednosti */
    IntegerLiteral *napraviInt(unsigned long long val) const;

    /* Pravljenje tacne istinitosne vrednosti */
    IntegerLiteral *napraviTrue() const;

    /* Pravljenje netacne istinitosne vrednosti */
    IntegerLiteral *napraviFalse() const;

    /* Pravljenje deklaracije uslovne promenljive */
    DeclStmt *napraviUslovnu(Decl *deklaracija,
                             const std::string &ime,
                             const bool pocetna) const;

    /* Pravljenje binarnog operatora */
    BinaryOperator *napraviBinarni(Expr *lhs, Expr *rhs,
                                   const BinaryOperator::Opcode &op,
                                   const CanQual<Type> &tip) const;

    /* Pravljenje izraza dodele */
    BinaryOperator *napraviDodelu(Expr *lhs, Expr *rhs) const;

    /* Pravljenje slozene naredbe */
    CompoundStmt *napraviSlozenu(const std::vector<Stmt *> &naredbe) const;

    /* Pravljenje uslovne naredbe */
    IfStmt *napraviIf(Expr *ako, Stmt *onda, Stmt *inace = nullptr) const;

    /* Pravljenje do petlje */
    DoStmt *napraviDo(Stmt *telo, Expr *uslov) const;

    /* Pravljenje while petlje */
    WhileStmt *napraviWhile(Expr *uslov, Stmt *telo) const;

    /* Pravljenje for petlje */
    ForStmt *napraviFor(Expr *uslov, Expr *korak, Stmt *telo) const;

    /* Pravljenje continue naredbe */
    ContinueStmt *napraviCont() const;

    /* Pravljenje break naredbe */
    BreakStmt *napraviBreak() const;

    /* Prebacivanje naredbe na hip */
    template <typename Naredba>
    Naredba *naHip(const Naredba &nar) const {
        const auto adresa = static_cast<Naredba *>(malloc(sizeof(nar)));
        memcpy(adresa, &nar, sizeof(nar)); return adresa;
    }

protected:
    /* Zasticeno cuvanje prepisivaca i konteksta */
    Rewriter &TheRewriter;
    ASTContext &TheASTContext;
};

#endif
