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

/* Postavljanje datoteka */
MTKTransformer::MTKTransformer(std::string &s, std::string &n)
    : stara(s), nova(n) {}

/* Staticko oslobadjanje memorije */
void MTKTransformer::oslobodi() {
    MTKContext::oslobodi();
}

/* Prijavljivanje greske u radu */
void MTKTransformer::greska(const std::string &poruka) {
    MTKContext::greska(poruka);
}

/* Obrada prema zeljenoj akciji; sustinski
 * boilerplate (sablonski) kod za rad sa AST */
void MTKTransformer::obradi(Akcija akcija) {
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
        case Akcija::While2Do:
            TheConsumer = new MTKConsumer<While2DoVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::Do2For:
            TheConsumer = new MTKConsumer<Do2ForVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::While2For:
            TheConsumer = new MTKConsumer<While2ForVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::PrepFor:
            TheConsumer = new MTKConsumer<PrepForVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::For2While:
            TheConsumer = new MTKConsumer<For2WhileVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::For2Do:
            TheConsumer = new MTKConsumer<For2DoVisitor>(TheRewriter, TheASTContext);
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
        case Akcija::PrepIf:
            TheConsumer = new MTKConsumer<PrepIfVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::PrepSwitch:
            TheConsumer = new MTKConsumer<PrepSwitchVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::If2Switch:
            TheConsumer = new MTKConsumer<If2SwitchVisitor>(TheRewriter, TheASTContext);
            break;
        case Akcija::Switch2If:
            TheConsumer = new MTKConsumer<Switch2IfVisitor>(TheRewriter, TheASTContext);
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
            std::ofstream izlaz(nova);
            if (!izlaz) MTKContext::greska(nemaDatoteke);

            /* Upisivanje rezultata */
            izlaz << std::string(RewriteBuf->begin(), RewriteBuf->end());
        } else if (stara == nova) {
            break;
        } else {
            /* Otvaranje ulazne i izlazne datoteke */
            std::ifstream ulaz(stara);
            std::ofstream izlaz(nova);
            if (!ulaz || !izlaz)
                MTKContext::greska(nemaDatoteke);

            /* Prepisivanje ulaza na izlaz */
            std::ostringstream buffer;
            buffer << ulaz.rdbuf();
            izlaz << buffer.str();
            break;
        }

        /* Zamena starog fajla */
        stara = nova;

        /* Iteracije i pripreme su jednoprolazni */
        if (akcija == Akcija::PrepFor ||
            akcija == Akcija::PrepIf ||
            akcija == Akcija::PrepSwitch ||
            akcija == Akcija::Rek2Iter ||
            akcija == Akcija::FinIter ||
            akcija == Akcija::FinRek)
            break;
    }
}
