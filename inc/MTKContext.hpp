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
    std::string stampaj(const Stmt * const s) const;

    /* Odredjivanje mesta naredbe u kodu */
    SourceRange odrediMesto(const Stmt * const s) const;

    /* Tekstualna zamena koda */
    void zameni(const Stmt * const stari, const Stmt * const novi);

    /* Prednja tekstualna dopuna koda */
    void dodajIspred(const Stmt * const stari, const Stmt * const novi);

    /* Zadnja tekstualna dopuna koda */
    void dodajIza(const Stmt * const stari, const Stmt * const novi);

    /* Pronalazak prvog slobodnog imena */
    std::string nadjiIme(const std::string &pocetno) const;

    /* Pravljenje nove deklaracije */
    VarDecl *napraviDecl(DeclContext *kontekst,
                         const CanQual<Type> &tip,
                         const std::string &ime);

    /* Pravljenje izraza deklaracije */
    DeclRefExpr *napraviDeclExpr(ValueDecl *deklaracija,
                                 const CanQual<Type> &tip);

    /* Pravljenje celobrojne vrednosti */
    IntegerLiteral *napraviInt(unsigned long long val);

    /* Pravljenje tacne istinitosne vrednosti */
    IntegerLiteral *napraviTrue();

    /* Pravljenje netacne istinitosne vrednosti */
    IntegerLiteral *napraviFalse();

    /* Pravljenje binarnog operatora */
    BinaryOperator *napraviBinarni(Expr *lhs, Expr *rhs,
                                   const BinaryOperator::Opcode &op,
                                   const CanQual<Type> &tip);

    /* Pravljenje izraza dodele */
    BinaryOperator *napraviDodelu(Expr *lhs, Expr *rhs,
                                  const CanQual<Type> &tip);

    /* Pravljenje slozene naredbe */
    CompoundStmt *napraviSlozenu(const std::vector<Stmt *> &naredbe);

    /* Pravljenje uslovne naredbe */
    IfStmt *napraviIf(Expr *ako, Stmt *onda, Stmt *inace = nullptr);

    /* Pravljenje do petlje */
    DoStmt *napraviDo(Stmt *telo, Expr *uslov);

    /* Pravljenje while petlje */
    WhileStmt *napraviWhile(Expr *uslov, Stmt *telo);

    /* Pravljenje for petlje */
    ForStmt *napraviFor(Expr *uslov, Expr *korak, Stmt *telo);

    /* Pravljenje continue naredbe */
    ContinueStmt *napraviCont();

    /* Pravljenje break naredbe */
    BreakStmt *napraviBreak();

protected:
    /* Zasticeno cuvanje prepisivaca i konteksta */
    Rewriter &TheRewriter;
    ASTContext &TheASTContext;
};

#endif
