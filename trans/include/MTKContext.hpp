#ifndef MTKCONTEXT_H
#define MTKCONTEXT_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include <unordered_set>

/* Upotreba Clangovog imenskog prostora */
using namespace clang;

/* Pomocna natklasa sa bitnim metodama */
class MTKContext {
public:
    /* Konstruktor inicijalizuje posetioca
     * prepisivacem i kontekstom parsiranja */
    MTKContext(Rewriter &R, ASTContext &A)
      : TheRewriter(R), TheASTContext(A) {}

    /* Staticko cuvanje posecenih funkcija */
    static std::unordered_set<std::string> *fje;

    /* Staticko oslobadjanje memorije */
    static void oslobodi();

    /* Prijavljivanje greske u radu */
    [[noreturn]] static void greska(const std::string &poruka);

    /* Tekstualna reprezentacija naredbe */
    std::string stampaj(const Stmt *const s) const;

    /* Odredjivanje mesta naredbe u kodu */
    SourceRange odrediMesto(const Stmt *const s,
                            bool dir = false) const;

    /* Tekstualna zamena koda */
    void zameni(const Stmt *const stari,
                const Stmt *const novi,
                bool dir = false) const;

    /* Prednja tekstualna dopuna koda */
    void dodajIspred(const Stmt *const stari,
                     const Stmt *const novi) const;

    /* Zadnja tekstualna dopuna koda */
    void dodajIza(const Stmt *const stari,
                  const Stmt *const novi) const;

    /* Tekstualna dopuna koda funkcijom */
    void dodajFunkciju(const FunctionDecl *const stara,
                       const FunctionDecl *const nova) const;

    /* Pronalazak prvog slobodnog imena */
    std::string nadjiIme(const std::string &pocetno) const;

    /* Pravljenje nove promenljive */
    VarDecl *napraviVar(DeclContext *kontekst,
                        const QualType &tip,
                        const std::string &ime) const;

    /* Pravljenje izraza deklaracije */
    DeclRefExpr *napraviDeclExpr(ValueDecl *dekl) const;

    /* Pravljenje izraza deklaracije */
    DeclRefExpr *napraviDeclExpr(DeclStmt *deknar) const;

    /* Pravljenje celobrojne vrednosti */
    IntegerLiteral *napraviInt(unsigned long long val) const;

    /* Pravljenje tacne istinitosne vrednosti */
    IntegerLiteral *napraviTrue() const;

    /* Pravljenje netacne istinitosne vrednosti */
    IntegerLiteral *napraviFalse() const;

    /* Pravljenje naredbe deklaracije */
    DeclStmt *napraviDeclStmt(VarDecl *var) const;

    /* Pravljenje naredbe deklaracije */
    DeclStmt *napraviDeclStmt(Decl *deklaracija,
                              const std::string &ime,
                              const QualType &tip,
                              Expr *pocetna = nullptr) const;

    /* Pravljenje deklaracije uslovne promenljive */
    DeclStmt *napraviUslovnu(Decl *deklaracija,
                             const std::string &ime,
                             const bool pocetna) const;

    /* Pravljenje izraza u zagradi */
    ParenExpr *napraviParen(Expr *izraz) const;

    /* Pravljenje unarnog operatora */
    UnaryOperator *napraviUnarni(Expr *izraz,
                                 const UnaryOperator::Opcode &op,
                                 const QualType &tip) const;

    /* Pravljenje logicke negacije */
    UnaryOperator *napraviNegaciju(Expr *izraz) const;

    /* Pravljenje referenciranja */
    UnaryOperator *napraviRef(DeclRefExpr *var) const;

    /* Pravljenje dereferenciranja */
    ParenExpr *napraviDeref(DeclRefExpr *var) const;

    /* Dohvatanje celobrojne vrednosti */
    Expr *dohvatiCelobrojnu(Expr *izraz) const;

    /* Dohvatanje istinitosne vrednosti */
    Expr *dohvatiIstinitost(Expr *izraz) const;

    /* Pravljenje binarnog operatora */
    BinaryOperator *napraviBinarni(Expr *lhs, Expr *rhs,
                                   const BinaryOperator::Opcode &op,
                                   const QualType &tip) const;

    /* Pravljenje izraza dodele */
    BinaryOperator *napraviDodelu(Expr *lhs, Expr *rhs) const;

    /* Pravljenje izraza jednakosti */
    BinaryOperator *napraviJednakost(Expr *lhs, Expr *rhs) const;

    /* Pravljenje izraza nejednakosti */
    BinaryOperator *napraviNejednakost(Expr *lhs, Expr *rhs) const;

    /* Pravljenje logicke konjunkcije */
    BinaryOperator *napraviKonjunkciju(Expr *lhs, Expr *rhs) const;

    /* Pravljenje logicke disjunkcije */
    BinaryOperator *napraviDisjunkciju(Expr *lhs, Expr *rhs) const;

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

    /* Pravljenje case klauze */
    CaseStmt *napraviCase(Expr *izraz, Stmt *naredba) const;

    /* Pravljenje default klauze */
    DefaultStmt *napraviDefault(Stmt *naredba) const;

    /* Pravljenje switch naredbe */
    SwitchStmt *napraviSwitch(Expr *uslov, Stmt *telo) const;

    /* Pravljenje funkcije */
    FunctionDecl *napraviFunkciju(DeclContext *kontekst,
                                  const QualType &tip,
                                  const std::string &ime) const;

    /* Pravljenje parametra */
    ParmVarDecl *napraviParam(VarDecl *var) const;

    /* Pravljenje funkcije */
    FunctionDecl *napraviFunkciju(DeclContext *kontekst,
                                  const QualType &tipRet,
                                  const std::string &ime,
                                  const std::vector<VarDecl *> &parms,
                                  Stmt *telo = nullptr) const;

    /* Pravljenje ref funkcije */
    FunctionDecl *napraviRefFunkciju(DeclContext *kontekst,
                                     const QualType &tipRet,
                                     const std::string &ime,
                                     const std::vector<VarDecl *> &parms,
                                     Stmt *telo = nullptr) const;

    /* Pravljenje poziva */
    CallExpr *napraviPoziv(FunctionDecl *funkcija,
                           const std::vector<Expr *> &args) const;

    /* Pravljenje ref poziva */
    CallExpr *napraviRefPoziv(FunctionDecl *funkcija,
                              const std::vector<DeclRefExpr *> &args) const;

    /* Pravljenje ref poziva */
    CallExpr *napraviRefPoziv(FunctionDecl *funkcija,
                              const std::vector<VarDecl *> &args) const;

    /* Pravljenje return naredbe */
    ReturnStmt *napraviReturn(Expr *izraz = nullptr) const;

    /* Pravljenje return naredbe */
    ReturnStmt *napraviReturn(bool vrednost) const;

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
