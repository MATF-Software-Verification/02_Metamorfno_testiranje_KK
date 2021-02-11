#include <QtTest>

#include <unistd.h>

#include "MTKTransformer.hpp"

/* Klasa za testiranje transformatora */
class MTKTest : public QObject {
    Q_OBJECT

/* Pozicioniranje u konstruktoru */
public:
    MTKTest();

/* Pomocne funkcije za provere */
private:
    void prevedi(const std::string &,
                 const std::string &);
    void akcija(const std::string &,
                const std::string &,
                const std::string &);

/* Slotovi za konkretne testove */
private Q_SLOTS:
    void petlje();
    void odmotavanje();
    void uslovi();
    void rekurzija();
    void umetanje();
    void greske();
};

/* Pozicioniranje u konstruktoru */
MTKTest::MTKTest() {
    chdir("../test");
}

/* Prevodjenje zadatog programa */
void MTKTest::prevedi(const std::string &in,
                      const std::string &out) {
    const auto prev = "clang-11 -O0 " + in
                      + " 2> /dev/null";
    std::system(prev.c_str());

    /* Cuvanje izlaza zadatog programa */
    const auto sav = "./a.out > " + out;
    std::system(sav.c_str());

    /* Brisanje izvrsive verzije programa */
    std::system("rm a.out");
}

/* Primena transformatora */
void MTKTest::akcija(const std::string &in,
                     const std::string &out,
                     const std::string &op) {
    /* Ciscenje izlaznog programa */
    const auto cist = "rm " + out + " 2> /dev/null";
    std::system(cist.c_str());

    /* Resetovanje statickih parametara */
    MTKTransformer::resetuj();

    /* Pravljenje izlaznog programa */
    const char *argv[] = {"trans", in.c_str(),
                          out.c_str(), op.c_str()};
    const int argc = sizeof argv / sizeof *argv;
    QVERIFY(!MTKTransformer::obradi(argc, argv));

    /* Prevodjenje izlaznog programa */
    prevedi(out, "output2.txt");

    /* Provera razlike izlaza */
    const auto rez =
        std::system("diff output.txt output2.txt -q");
    QVERIFY(WIFEXITED(rez) && !WEXITSTATUS(rez));

    /* Ciscenje izlaza programa */
    std::system("rm output2.txt");
}

/* Provera rada sa petljama */
void MTKTest::petlje() {
    chdir("petlje");

    /* Prevodjenje ulaznog programa */
    prevedi("whilefor.c", "output.txt");

    /* Prva iteracija tranformacija */
    akcija("whilefor.c", "samodo.c", "do");
    akcija("whilefor.c", "samofor.c", "for");
    akcija("whilefor.c", "samowhile.c", "while");

    /* Druga iteracija transformacija */
    akcija("samodo.c", "samodo2.c", "do");
    akcija("samodo.c", "samofor2.c", "for");
    akcija("samodo.c", "samowhile2.c", "while");

    /* Vracanje na pocetnu poziciju */
    chdir("..");
}

/* Provera rada sa odmotavanjem */
void MTKTest::odmotavanje() {
    chdir("odmotavanje");

    /* Prevodjenje ulaznog programa */
    prevedi("jednostruke.c", "output.txt");

    /* Prva iteracija tranformacija */
    akcija("jednostruke.c", "bezpromene.c", "o0");
    akcija("jednostruke.c", "trostruke.c", "o2");

    /* Vracanje na pocetnu poziciju */
    chdir("..");
}

/* Provera rada sa uslovima */
void MTKTest::uslovi() {
    chdir("uslovi");

    /* Prevodjenje ulaznog programa */
    prevedi("ifswitch.c", "output.txt");

    /* Prva iteracija tranformacija */
    akcija("ifswitch.c", "samoif.c", "if");
    akcija("ifswitch.c", "samoswitch.c", "switch");

    /* Druga iteracija tranformacija */
    akcija("samoif.c", "samoswitch2.c", "switch");
    akcija("samoswitch.c", "samoif2.c", "if");

    /* Vracanje na pocetnu poziciju */
    chdir("..");
}

/* Provera rada sa rekurzijom */
void MTKTest::rekurzija() {
    chdir("rekurzija");

    /* Prevodjenje ulaznog programa */
    prevedi("rekiter.c", "output.txt");

    /* Prva iteracija tranformacija */
    akcija("rekiter.c", "samoiter.c", "iter");
    akcija("rekiter.c", "samorek.c", "rek");

    /* Prva iteracija tranformacija */
    akcija("samoiter.c", "samorek2.c", "rek");
    akcija("samorek.c", "samoiter2.c", "iter");

    /* Vracanje na pocetnu poziciju */
    chdir("..");
}

/* Provera rada sa umetanjem */
void MTKTest::umetanje() {
    chdir("umetanje");

    /* Prevodjenje ulaznog programa */
    prevedi("normal.c", "output.txt");

    /* Prva iteracija tranformacija */
    akcija("normal.c", "bloated.c", "u3");

    /* Vracanje na pocetnu poziciju */
    chdir("..");
}

/* Provera rada sa greskama */
void MTKTest::greske() {
    /* Manjak argumenata komandne linije */
    {
        MTKTransformer::resetuj();
        const char *argv[] = {"trans"};
        const int argc = sizeof argv / sizeof *argv;
        QVERIFY(MTKTransformer::obradi(argc, argv));
    }

    /* Pogresna opcija komandne linije */
    {
        MTKTransformer::resetuj();
        const char *argv[] = {"trans", "", "", "yo"};
        const int argc = sizeof argv / sizeof *argv;
        QVERIFY(MTKTransformer::obradi(argc, argv));
    }

    /* Pogresna opcija komandne linije */
    {
        MTKTransformer::resetuj();
        const char *argv[] = {"trans", "", "", "ok"};
        const int argc = sizeof argv / sizeof *argv;
        QVERIFY(MTKTransformer::obradi(argc, argv));
    }

    /* Pogresna opcija komandne linije */
    {
        MTKTransformer::resetuj();
        const char *argv[] = {"trans", "", "", "uk"};
        const int argc = sizeof argv / sizeof *argv;
        QVERIFY(MTKTransformer::obradi(argc, argv));
    }

    /* Nova datoteka vec postoji */
    {
        MTKTransformer::resetuj();
        const char *argv[] = {"trans", "petlje/whilefor.c",
                              "uslovi/ifswitch.c", "do"};
        const int argc = sizeof argv / sizeof *argv;
        QVERIFY(MTKTransformer::obradi(argc, argv));
    }
}

/* Automatski main sa jednom klasom */
QTEST_APPLESS_MAIN(MTKTest)

/* Moc sa svim potrebnim informacijama */
#include "main.moc"
