#ifndef OOP_PROJEKAT_LINIJAPREVOZA_H
#define OOP_PROJEKAT_LINIJAPREVOZA_H

#include "Izuzeci.h"

#include <limits>
#include <map>
#include <string>
#include <vector>

const int INF = 36000;
const int MAX_CEKANJE = 100; //minuta

class LinijaPrevoza {
public:
    LinijaPrevoza()=default;
    ~LinijaPrevoza()=default;
    LinijaPrevoza(const LinijaPrevoza& cvor)=delete;
    LinijaPrevoza& operator= (const LinijaPrevoza& cvor) = delete;

    std::string uzmiNaziv();
    std::vector<int> uzmiSifreStajalista();
    int uzmiBrojPolazaka() const;

    void ucitajLiniju(const std::string& linija_fajla, const std::map <int, std::string>& stajalista);
    bool prolaziKroz(int stajaliste);

    int doSledecegProlaska(int stajaliste, int trenutno_vreme);
    int prethodno(int stajaliste);
    int sledece(int stajaliste);
private:
    std::string oznaka_linije;

    // u minutima :
    int prvi_polazak;
    int poslednji_polazak;
    int razmak_izmedju_polazaka;

    std::vector<int> sifre_stajalista;
    int broj_polazaka;

    static std::string procitajRec(const std::string& linija, int& pozicija);
    static bool jeBroj(const std::string& s);
};


#endif //OOP_PROJEKAT_LINIJAPREVOZA_H
