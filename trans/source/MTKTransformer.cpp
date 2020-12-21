#include "While2DoVisitor.hpp"
#include "Do2ForVisitor.hpp"
#include "While2ForVisitor.hpp"
#include "For2WhileVisitor.hpp"
#include "For2DoVisitor.hpp"
#include "PrepForVisitor.hpp"
#include "Rek2IterVisitor.hpp"
#include "FinIterVisitor.hpp"
#include "Iter2RekVisitor.hpp"
#include "FinRekVisitor.hpp"
#include "PrepIfVisitor.hpp"
#include "PrepSwitchVisitor.hpp"
#include "If2SwitchVisitor.hpp"
#include "Switch2IfVisitor.hpp"

#include "MTKTransformer.hpp"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/Support/Host.h"

#include <fstream>
#include <sstream>

/* Inicijalizacija transformatora */
MTKTransformer::MTKTransformer(const std::string &s,
                               const std::string &n)
    : stara(s), nova(n) {
    /* Nije sigurno da bude ista datoteka */
    if (stara == nova) greska(istaDatoteka);

    /* Stara mora da bude validan kod */
    proveri();

    /* Nova ne sme da postoji vec */
    std::ifstream ulaz(nova);
    if (ulaz) greska(postojiDatoteka);

    /* Postavljanje skupa funkcija */
    MTKContext::postaviFje(fje);

    /* Postavljanje izlazne datoteke */
    MTKContext::postaviDat(nova);
}

/* Prijavljivanje greske u radu */
void MTKTransformer::greska(const std::string &poruka) {
    MTKContext::greska(poruka);
}

/* Primena zeljene izmene koda; sustinski je
 * boilerplate (sablonski) kod za rad sa AST */
void MTKTransformer::primeni(Izmena izmena) {
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
        const auto FileInOpt = FileMgr.getFile(stara);
        if (!FileInOpt) greska(nemaDatoteke);
        const auto FileIn = FileInOpt.get();

        /* Postavljanje prosledjenog fajla za ulazni */
        SourceMgr.setMainFileID(
                    SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
        TheCompInst.getDiagnosticClient().BeginSourceFile(
                    TheCompInst.getLangOpts(), &ThePreprocessor);

        /* Odabir transformatora prema izmeni */
        auto &TheASTContext = TheCompInst.getASTContext();
        ASTConsumer* TheConsumer;
        switch (izmena) {
        case Izmena::While2Do:
            TheConsumer = new MTKConsumer<While2DoVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::Do2For:
            TheConsumer = new MTKConsumer<Do2ForVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::While2For:
            TheConsumer = new MTKConsumer<While2ForVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::PrepFor:
            TheConsumer = new MTKConsumer<PrepForVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::For2While:
            TheConsumer = new MTKConsumer<For2WhileVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::For2Do:
            TheConsumer = new MTKConsumer<For2DoVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::Rek2Iter:
            TheConsumer = new MTKConsumer<Rek2IterVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::FinIter:
            TheConsumer = new MTKConsumer<FinIterVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::Iter2Rek:
            TheConsumer = new MTKConsumer<Iter2RekVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::FinRek:
            TheConsumer = new MTKConsumer<FinRekVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::PrepIf:
            TheConsumer = new MTKConsumer<PrepIfVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::PrepSwitch:
            TheConsumer = new MTKConsumer<PrepSwitchVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::If2Switch:
            TheConsumer = new MTKConsumer<If2SwitchVisitor>(TheRewriter, TheASTContext);
            break;
        case Izmena::Switch2If:
            TheConsumer = new MTKConsumer<Switch2IfVisitor>(TheRewriter, TheASTContext);
            break;
        }

        /* Parsiranje i obrada AST stabla */
        ParseAST(ThePreprocessor, TheConsumer, TheASTContext);
        delete TheConsumer;

        /* Upisivanje novog koda iz bafera; u slucaju da nema
         * izmena, prosto prepisivanje starog koda kao novog */
        const auto RewriteBuf =
                TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
        if (RewriteBuf) {
            /* Otvaranje izlazne datoteke */
            std::ofstream izlaz(nova);
            if (!izlaz) greska(nemaDatoteke);

            /* Upisivanje rezultata */
            izlaz << std::string(RewriteBuf->begin(), RewriteBuf->end());
        } else if (stara == nova) {
            if (izmena != Izmena::FinRek ||
                FinRekVisitor::imaloPosla()) break;
        } else {
            /* Otvaranje ulazne i izlazne datoteke */
            std::ifstream ulaz(stara);
            std::ofstream izlaz(nova);
            if (!ulaz || !izlaz)
                greska(nemaDatoteke);

            /* Prepisivanje ulaza na izlaz */
            std::ostringstream buffer;
            buffer << ulaz.rdbuf();
            izlaz << buffer.str();
            break;
        }

        /* Zamena starog fajla */
        stara = nova;

        /* Provera da li je sve u redu */
        if (izmena == Izmena::Iter2Rek)
            proveri();

        /* Iteracije i pripreme su jednoprolazni */
        if (izmena == Izmena::PrepFor ||
            izmena == Izmena::PrepIf ||
            izmena == Izmena::PrepSwitch ||
            izmena == Izmena::Rek2Iter ||
            izmena == Izmena::FinIter)
            break;
    }
}

/* Provera validnosti ulaza */
void MTKTransformer::proveri() const {
    /* Pokusaj prevodjenja ulaza */
    std::ostringstream buffer;
    buffer << "clang-11 " << stara << " 2>/dev/null";
    const auto ret
        = std::system(buffer.str().c_str());

    /* Ciscenje za sobom */
    std::system("rm a.out 2>/dev/null");

    /* Prijava greske ako nesto nije u redu */
    if (!WIFEXITED(ret) || WEXITSTATUS(ret))
        greska(losUlaz);
}
