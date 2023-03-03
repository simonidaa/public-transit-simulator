#ifndef OOP_PROJEKAT_MREZAGRADSKOGPREVOZA_H
#define OOP_PROJEKAT_MREZAGRADSKOGPREVOZA_H

#include "Cvor.h"
#include "Izuzeci.h"
#include "LinijaPrevoza.h"

#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>


class MrezaGradskogPrevoza {
public:
    MrezaGradskogPrevoza()=default;
    MrezaGradskogPrevoza(const MrezaGradskogPrevoza& m)=delete;
    MrezaGradskogPrevoza& operator= (const MrezaGradskogPrevoza& m) = delete;
    ~MrezaGradskogPrevoza();

    void pokreniSimulator();

    void ucitajStajalista(const std::string& putanja_fajla);
    void ucitajLinije(const std::string& putanja_fajla);

    void prikazStajalista(int stajaliste);
    void prikazLinije(const std::string& oznaka_linije);
    void prikazStatistikeLinije(const std::string& oznaka_linije);

    void pronadjiPutanju(int pocetak, int kraj, const std::string& vreme);
    void pronadjiNajbrzuPutanju(int pocetak, int kraj, const std::string& vreme);
    void pronadjiNajmanjePresedanja(int pocetak, int kraj, const std::string& vreme);
private:
    std::map <int, std::string> stajalista;
    std::vector<LinijaPrevoza*> linije;

    void ucitajStajaliste(const std::string& linija_fajla);
    static std::string procitajRec(const std::string& linija, int& pozicija);
    static bool jeBroj(const std::string& s);
    static bool imajuZajednicko(LinijaPrevoza* prva, LinijaPrevoza* druga);
    static int procitajVreme(const std::string& vreme);
    static int procitajBroj();

    LinijaPrevoza* uzmiLiniju(const std::string& oznaka_linije);
    std::vector<Cvor *> generisiGraf();

    void pokreniOpcije();
    static void ispisPutanje(Cvor* krajnji, const std::string& ime_fajla, const std::string& poruka);
    static void izlazniFajl(const std::string& ime_fajla, const std::string& ispis);

    static Cvor* pronadjiStanicu(int sifra, std::vector<Cvor*> &graf);
};



#endif //OOP_PROJEKAT_MREZAGRADSKOGPREVOZA_H
