#include "For2GotoVisitor.hpp"
#include "Assert.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * for (init; cond; inc)
 *   telo;
 * ---------------------
 * init;
 * for_loop_begin_{unique_id}:
 * if (!cond)
 *    goto for_loop_end_{unique_id};
 * telo;
 * inc;
 * goto for_loop_begin_unique_id;
 * for_loop_end_{unique_id};
 ***********************/
#include<iostream>
#include <memory>
#if 0
std::unordered_map<size_t, For2GotoVisitor::ForLoopLabels> For2GotoVisitor::forLoopGotoLabels = {};
bool For2GotoVisitor::zamenjeniSviBreakIContinueStmt = false;
size_t For2GotoVisitor::jedinstveniIdLabelePetlji_ = 0;

std::unordered_set<std::string> For2GotoVisitor::vidjeneFunkcije;
#endif

std::unordered_map<std::string, std::unique_ptr<For2GotoVisitor::FunctionData>> For2GotoVisitor::podaciFunkcije;

/* Posetilac koji for pretvara u while */
bool For2GotoVisitor::TraverseFunctionDecl(FunctionDecl *f)
{
    kontekstFunkcijaDecl_ = f;

    imeTrenutneFunkcije = f->getDeclName().getAsString();
    std::string imeFUnkcije = imeTrenutneFunkcije;
    std::cerr << __PRETTY_FUNCTION__ << '\n' << imeTrenutneFunkcije << '\n';
    bool rezultat = true;
    if (f->hasBody()) {
        MTKVisitor::izracunajDecu(f->getBody());
        auto& entry = podaciFunkcije[imeTrenutneFunkcije];
        if (!entry) {
            entry = std::make_unique<For2GotoVisitor::FunctionData>();
        }

        if (entry->zamenjeniSviBreakIContinueStmt && entry->prviProlaz) {
            entry->trenutniRedniBrojPetljeUFunkcijiZaObradu = 0;
            entry->prviProlaz = false;
        }
        rezultat = MTKVisitor::TraverseFunctionDecl(f);
        Assert(imeFUnkcije == imeTrenutneFunkcije, "Promenilo se ime funkcije, deklaracija u deklaraciji");
        kontekstFunkcijaDecl_ = f;
        imeTrenutneFunkcije = f->getDeclName().getAsString();
        entry->zamenjeniSviBreakIContinueStmt = true;
    }
    return rezultat;
}

bool For2GotoVisitor::VisitForStmt(ForStmt *s) {
    Assert(s != nullptr, "nema ForStmt");

    auto& entry = podaciFunkcije.find(imeTrenutneFunkcije)->second;
    Assert(entry != nullptr, "Mora postojati podataka u trenutnoj funkciji koja se obradjuje!");

    std::cerr << __PRETTY_FUNCTION__ << '\n' << imeTrenutneFunkcije << '\n';
    std::cerr << "for:" << s->getForLoc().printToString(TheRewriter.getSourceMgr()) << '\n';
    if (entry->zamenjeniSviBreakIContinueStmt) {
        // Zameni for petlju u goto
        std::vector<Stmt*> noveNaredbe;
        if (auto initStmt = s->getInit()) {
            noveNaredbe.emplace_back(initStmt);
        }
        size_t id = ++(entry->trenutniRedniBrojPetljeUFunkcijiZaObradu);
        std::cerr << "zamenjeniVsiBreakiContinueId: " << id << '\n';
        auto labele = entry->labeleZaPetlju.find(id);
        Assert(labele != entry->labeleZaPetlju.end(), "Nema labeli");

        auto labelaPocetakPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.nazivLabelePocetakPetlje);
        auto labelaKrajPetlje = napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.nazivLabeleKrajPetlje);

        noveNaredbe.emplace_back(labelaPocetakPetlje);

        if (auto condExpr = s->getCond()) {
            auto ifCond = napraviIf(napraviNegaciju(condExpr), napraviGoto(labelaKrajPetlje));
            Assert(ifCond, "Nije napravljen ifCond");
            noveNaredbe.emplace_back(ifCond);
        }

        if (auto loopBody = s->getBody()) {
            noveNaredbe.emplace_back(napraviSlozenu({loopBody}));
        }

        noveNaredbe.emplace_back(napraviLabelStmt(kontekstFunkcijaDecl_, labele->second.nazivLabeleIncPetlje));
        if (auto loopIncr = s->getInc()) {
            noveNaredbe.emplace_back(loopIncr);
        }
        noveNaredbe.emplace_back(napraviGoto(labelaPocetakPetlje));
        noveNaredbe.emplace_back(labelaKrajPetlje);
        zameni(s, napraviSlozenu(noveNaredbe));
        return false; // zaustavi oblizak
    } else if(entry->prviProlaz) {
        //if (entry->labeleZaPetlju.find(entry->trenutniRedniBrojPetljeUFunkcijiZaObradu + 1) == entry->labeleZaPetlju.end()) {
        // Prvi put
        // Ako prvi put vidimo ForStmt napravi jedinstevene goto labele
        // TODO(Marko): Osiguraj jedinstvenost naziva labele
        size_t id = ++entry->trenutniRedniBrojPetljeUFunkcijiZaObradu;
        std::cerr << "prviProlazId: " << id << '\n';
        std::string id_string = std::to_string(id);
        std::string labelaPocetakPetljeNaziv("labela_for_loop_begin_");
        labelaPocetakPetljeNaziv.append(id_string);

        std::string labelaKrajPetljeNaziv("labela_for_loop_end_");
        labelaKrajPetljeNaziv.append(id_string);

        std::string labelaIncPetljeNaziv("labela_for_loop_inc_");
        labelaIncPetljeNaziv.append(id_string);

        entry->labeleZaPetlju[id] = ForLoopLabels{labelaPocetakPetljeNaziv, labelaIncPetljeNaziv, labelaKrajPetljeNaziv};
        return true; // nastavi oblizak
    }
    return true;
}


bool For2GotoVisitor::VisitBreakStmt(BreakStmt *s)
{
    auto &entry = podaciFunkcije.find(imeTrenutneFunkcije)->second;
    if (entry->zamenjeniSviBreakIContinueStmt)
        return true;

    for (auto roditeljIter = rods.find(s); roditeljIter != std::end(rods); roditeljIter = rods.find(roditeljIter->second)) {
        auto roditelj = roditeljIter->second;
        if (const ForStmt* forStmtRoditelj = dyn_cast<ForStmt>(roditelj))	{
           // Zameni break sa goto whileStmtRoditelj end loop labelom
            auto labelaKrajPetljeNaziv = entry->labeleZaPetlju
                    .find(entry->trenutniRedniBrojPetljeUFunkcijiZaObradu)->second.nazivLabeleKrajPetlje;
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcijaDecl_, labelaKrajPetljeNaziv)));
            break;
        } else if (isa<SwitchStmt>(*roditelj) || isa<WhileStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}

bool For2GotoVisitor::VisitContinueStmt(ContinueStmt *s)
{
    auto &entry = podaciFunkcije.find(imeTrenutneFunkcije)->second;
    if (entry->zamenjeniSviBreakIContinueStmt)
        return true;

    for (auto roditeljIter = rods.find(s); roditeljIter != std::end(rods); roditeljIter = rods.find(roditeljIter->second)) {
        auto roditelj = roditeljIter->second;
        if (const ForStmt* forStmtRoditelj = dyn_cast<ForStmt>(roditelj)) {
            // Zameni continue sa goto whileStmtROiditelj begin loop labelonm
            auto labelaIncPetljeNaziv = entry->labeleZaPetlju
                    .find(entry->trenutniRedniBrojPetljeUFunkcijiZaObradu)->second.nazivLabeleIncPetlje;
            zameni(s, napraviGoto(napraviLabelStmt(kontekstFunkcijaDecl_, labelaIncPetljeNaziv)));
            break;
        } else if (isa<WhileStmt>(*roditelj) || isa<DoStmt>(*roditelj)) {
            break;
        }
    }
    return true;
}



