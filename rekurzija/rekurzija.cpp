#include "Do2ForVisitor.hpp"
#include "PrepForVisitor.hpp"
#include "For2WhileVisitor.hpp"
#include "Rek2IterVisitor.hpp"
#include "FinIterVisitor.hpp"
#include "Iter2RekVisitor.hpp"
#include "FinRekVisitor.hpp"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/Support/Host.h"

#include <fstream>
#include <sstream>

/* Enumeracija akcija */
enum class Akcija {
    Do2For,
    PrepFor,
    For2While,
    Rek2Iter,
    FinIter,
    Iter2Rek,
    FinRek
};

/* Nacin upotrebe programa */
static std::string *stari, *novi, *rekurzija;
static const auto upotreba = "Upotreba: ./rekurzija <ulaz> <izlaz> <iter|rek>";
static const auto nemaDatoteke = "Neuspelo otvaranje datoteke!";

/* Obrada prema zeljenoj akciji; sustinski
 * boilerplate (sablonski) kod za rad sa AST */
static void obradi(const Akcija &akcija) {
    /* Parsiranje dokle god ima promena */
    for(;;) {
        /* Pravljenje i inicijalizacija prevodioca */
        CompilerInstance TheCompInst;
        TheCompInst.createDiagnostics();
        auto &TheDiagnostics = TheCompInst.getDiagnostics();
        TheDiagnostics.setSuppressAllDiagnostics(true);

        /* Postavljanje podataka o ciljnoj platformi */
        auto TO = std::make_shared<TargetOptions>();
        TO->Triple = llvm::sys::getDefaultTargetTriple();
        auto TI = TargetInfo::CreateTargetInfo(TheDiagnostics, TO);
        TheCompInst.setTarget(TI);

        /* Pravljenje upravljaca datoteka */
        TheCompInst.createFileManager();
        auto &FileMgr = TheCompInst.getFileManager();

        /* Pravljenje upravljaca izvora */
        TheCompInst.createSourceManager(FileMgr);
        auto &SourceMgr = TheCompInst.getSourceManager();

        /* Pravljenje pretprocesora */
        TheCompInst.createPreprocessor(TU_Module);
        auto &ThePreprocessor = TheCompInst.getPreprocessor();

        /* Pravljenje AST konteksta */
        TheCompInst.createASTContext();

        /* Pravljenje prepisivaca koda */
        Rewriter TheRewriter;
        TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

        /* Citanje prosledjenog fajla */
        const auto FileInOpt = FileMgr.getFile(*stari);
        if (!FileInOpt) MTKContext::greska(nemaDatoteke);
        const auto FileIn = FileInOpt.get();

        /* Postavljanje prosledjenog fajla za ulazni */
        SourceMgr.setMainFileID(
            SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
        TheCompInst.getDiagnosticClient().BeginSourceFile(
            TheCompInst.getLangOpts(), &ThePreprocessor);

        /* Odabir petlje u koju se ostale menjaju */
        auto &TheASTContext = TheCompInst.getASTContext();
        ASTConsumer* TheConsumer;
        switch (akcija) {
        case Akcija::Do2For:
            TheConsumer = new MTKConsumer<Do2ForVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::PrepFor:
            TheConsumer = new MTKConsumer<PrepForVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::For2While:
            TheConsumer = new MTKConsumer<For2WhileVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::Rek2Iter:
            TheConsumer = new MTKConsumer<Rek2IterVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::FinIter:
            TheConsumer = new MTKConsumer<FinIterVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::Iter2Rek:
            TheConsumer = new MTKConsumer<Iter2RekVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::FinRek:
            TheConsumer = new MTKConsumer<FinRekVisitor>(TheRewriter, TheASTContext);
            break;
        }

        /* Parsiranje i obrada AST stabla */
        ParseAST(ThePreprocessor, TheConsumer, TheASTContext);
        delete TheConsumer;

        /* Upisivanje novog koda iz bafera; u slucaju da nema
         * izmena, prosto prepisivanje starog koda kao novog */
        const auto *RewriteBuf =
            TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
        if (RewriteBuf) {
            /* Otvaranje izlazne datoteke */
            std::ofstream izlaz(*novi);
            if (!izlaz) MTKContext::greska(nemaDatoteke);

            /* Upisivanje rezultata */
            izlaz << std::string(RewriteBuf->begin(), RewriteBuf->end());
        } else if (*stari == *novi) {
            break;
        } else {
            /* Otvaranje ulazne i izlazne datoteke */
            std::ifstream ulaz(*stari);
            std::ofstream izlaz(*novi);
            if (!ulaz || !izlaz)
                MTKContext::greska(nemaDatoteke);

            /* Prepisivanje ulaza na izlaz */
            std::ostringstream buffer;
            buffer << ulaz.rdbuf();
            izlaz << buffer.str();
            break;
        }

        /* Zamena starog fajla */
        *stari = *novi;

        /* Iteracije i pripreme su jednoprolazni */
        if (akcija == Akcija::PrepFor ||
            akcija == Akcija::Rek2Iter ||
            akcija == Akcija::FinIter ||
            akcija == Akcija::FinRek)
            break;
    }
}

/* Glavna funkcija aplikacije */
int main(int argc, char *argv[]) {
    /* Prekid pogresno pokrenutog programa */
    if (argc != 4) MTKContext::greska(upotreba);

    /* Citanje argumenata */
    stari = new std::string(argv[1]);
    novi = new std::string(argv[2]);
    rekurzija = new std::string(argv[3]);

    /* Obrada zeljene transformacije */
    if (*rekurzija == "iter") {
        obradi(Akcija::Rek2Iter);
        obradi(Akcija::FinIter);
    } else if (*rekurzija == "rek") {
        obradi(Akcija::Do2For);
        obradi(Akcija::PrepFor);
        obradi(Akcija::For2While);
        obradi(Akcija::Iter2Rek);
        obradi(Akcija::FinRek);
    /* Prekid pogresno pokrenutog programa */
    } else MTKContext::greska(upotreba);

    /* Lepo formatiranje novog koda */
    std::ostringstream buffer;
    buffer << "clang-format -i " << *novi;
    std::system(buffer.str().c_str());

    /* Oslobadjanje memorije */
    delete stari;
    delete novi;
    delete rekurzija;
    delete MTKContext::fje;

    /* Normalno zavrsavanje programa */
    exit(EXIT_SUCCESS);
}
