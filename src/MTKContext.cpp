#include "MTKContext.hpp"

#include "clang/Lex/Preprocessor.h"

#include <climits>

/* Tekstualna reprezentacija naredbe */
std::string MTKContext::stampaj(const clang::Stmt * const s) const {
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

/* Odredjivanje mesta naredbe u kodu */
SourceRange MTKContext::odrediMesto(const Stmt * const s) const {
    /* Granicne oznake u kodu */
    const auto start = s->getSourceRange().getBegin();
    const auto end = s->getSourceRange().getEnd();

    /* Dohvatanje poslednjeg tokena */
    Token tok;
    Lexer::getRawToken(end, tok, TheRewriter.getSourceMgr(),
                       TheRewriter.getLangOpts());
    const std::string ime = tok.getName();

    /* Racunanje offseta osim ukoliko je kraj slozene naredbe; tada
     * je tacka-zapeta ili zatvorena velika zagrada suvisni token */
    const auto offset = Lexer::MeasureTokenLength(end,
                                                  TheRewriter.getSourceMgr(),
                                                  TheRewriter.getLangOpts())
            + (ime != "r_brace" && ime != "semi");

    /* Vracanje tacno izracunatog mesta */
    return SourceRange(start, end.getLocWithOffset(static_cast<int>(offset)));
}

/* Tekstualna zamena koda */
void MTKContext::zameni(const Stmt * const stari, const Stmt * const novi) {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari);

    /* Tekstualna reprezentacija novog iskaza */
    const auto stmt = stampaj(novi);

    /* Promena teksta na izracunatom mestu */
    TheRewriter.ReplaceText(mesto, stmt);
}

/* Prednja tekstualna dopuna koda */
void MTKContext::dodajIspred(const Stmt * const stari, const Stmt * const novi) {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari);

    /* Tekstualna reprezentacija novog iskaza */
    const auto stmt = "{" + stampaj(novi);

    /* Dodavanje teksta na izracunatom mestu */
    TheRewriter.InsertTextBefore(mesto.getBegin(), stmt);
}

/* Zadnja tekstualna dopuna koda */
void MTKContext::dodajIza(const Stmt * const stari, const Stmt * const novi) {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari);

    /* Tekstualna reprezentacija novog iskaza */
    const auto stmt = stampaj(novi) + "}";

    /* Dodavanje teksta na izracunatom mestu */
    TheRewriter.InsertTextAfter(mesto.getEnd(), stmt);
}

/* Pronalazak prvog slobodnog imena */
std::string MTKContext::nadjiIme(const std::string &pocetno) const {
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
VarDecl *MTKContext::napraviDecl(DeclContext *kontekst,
                                 const CanQual<Type> &tip,
                                 const std::string &ime) {
    return VarDecl::Create(TheASTContext, kontekst,
                           SourceLocation(), SourceLocation(),
                           &TheASTContext.Idents.getOwn(ime),
                           tip, nullptr, SC_None);
}

/* Pravljenje izraza deklaracije */
DeclRefExpr *MTKContext::napraviDeclExpr(ValueDecl *deklaracija,
                                         const CanQual<Type> &tip) {
    return DeclRefExpr::Create(TheASTContext, NestedNameSpecifierLoc(),
                               SourceLocation(), deklaracija, true,
                               SourceLocation(), tip, VK_LValue);
}

/* Pravljenje celobrojne vrednosti */
IntegerLiteral *MTKContext::napraviInt(unsigned long long val) {
    const auto tip = TheASTContext.IntTy;
    llvm::APInt APValue(static_cast<unsigned>(TheASTContext.getTypeSize(tip)), val);
    return IntegerLiteral::Create(TheASTContext, APValue,
                                  tip, SourceLocation());
}

/* Pravljenje tacne istinitosne vrednosti */
IntegerLiteral *MTKContext::napraviTrue() {
    return napraviInt(1);
}

/* Pravljenje netacne istinitosne vrednosti */
IntegerLiteral *MTKContext::napraviFalse() {
    return napraviInt(0);
}

/* Pravljenje binarnog operatora */
BinaryOperator *MTKContext::napraviBinarni(Expr *lhs, Expr *rhs,
                                           const BinaryOperator::Opcode &op,
                                           const CanQual<Type> &tip) {
    return BinaryOperator::Create(TheASTContext, lhs, rhs, op,
                                  tip, VK_RValue, OK_Ordinary,
                                  SourceLocation(), FPOptionsOverride());
}

/* Pravljenje izraza dodele */
BinaryOperator *MTKContext::napraviDodelu(Expr *lhs, Expr *rhs,
                                          const CanQual<Type> &tip) {
    return napraviBinarni(lhs, rhs, BO_Assign, tip);
}

/* Pravljenje slozene naredbe */
CompoundStmt *MTKContext::napraviSlozenu(const std::vector<Stmt *> &naredbe) {
    return CompoundStmt::Create(TheASTContext, naredbe,
                                SourceLocation(), SourceLocation());
}

/* Pravljenje uslovne naredbe */
IfStmt *MTKContext::napraviIf(Expr *ako, Stmt *onda, Stmt *inace) {
    return IfStmt::Create(TheASTContext, SourceLocation(),
                          false, nullptr, nullptr, ako,
                          onda, SourceLocation(), inace);
}

/* Pravljenje do petlje */
DoStmt *MTKContext::napraviDo(Stmt *telo, Expr *uslov) {
    DoStmt petlja(telo, uslov, SourceLocation(),
                  SourceLocation(), SourceLocation());
    auto *adresa = static_cast<DoStmt *>(malloc(sizeof(petlja)));
    memcpy(adresa, &petlja, sizeof(petlja));
    return adresa;
}

/* Pravljenje while petlje */
WhileStmt *MTKContext::napraviWhile(Expr *uslov, Stmt *telo) {
    return WhileStmt::Create(TheASTContext, nullptr, uslov, telo,
                             SourceLocation(), SourceLocation(), SourceLocation());
}

/* Pravljenje for petlje */
ForStmt *MTKContext::napraviFor(Expr *uslov, Expr *korak, Stmt *telo) {
    ForStmt petlja(TheASTContext, nullptr, uslov, nullptr, korak, telo,
                   SourceLocation(), SourceLocation(), SourceLocation());
    auto *adresa = static_cast<ForStmt *>(malloc(sizeof(petlja)));
    memcpy(adresa, &petlja, sizeof(petlja));
    return adresa;
}

/* Pravljenje continue naredbe */
ContinueStmt *MTKContext::napraviCont() {
    ContinueStmt cont{SourceLocation()};
    auto *adresa = static_cast<ContinueStmt *>(malloc(sizeof(cont)));
    memcpy(adresa, &cont, sizeof(cont));
    return adresa;
}

/* Pravljenje break naredbe */
BreakStmt *MTKContext::napraviBreak() {
    BreakStmt br{SourceLocation()};
    auto *adresa = static_cast<BreakStmt *>(malloc(sizeof(br)));
    memcpy(adresa, &br, sizeof(br));
    return adresa;
}
