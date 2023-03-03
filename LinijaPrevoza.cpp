#include "LinijaPrevoza.h"

void LinijaPrevoza::ucitajLiniju(const std::string& linija_fajla, const std::map<int, std::string>& stajalista) {
    // linija fajla je u formatu : oznaka_linije [prvi_polazak-poslednji_polazak#razmak_između_polazaka] stajališta
    int poz = 0;
    std::string pom;

    this->oznaka_linije = procitajRec(linija_fajla, poz);
    if (linija_fajla[poz]==' ') poz++;
    else throw PogresanUnos(poz+1);

    if (linija_fajla[poz]=='[') poz++;
    else throw PogresanUnos(poz+1);

    pom = linija_fajla.substr(poz, 2);
    poz+=2;
    if (jeBroj(pom) && std::stoi(pom) < 24) this->prvi_polazak = std::stoi(pom) * 60 ;
        // Los unos - satnica mora biti celobrojna vrednost, manja od 24
    else throw PogresnoVreme('S');


    if (linija_fajla[poz]==':') poz++;
    else throw PogresanUnos(poz+1);

    pom = linija_fajla.substr(poz, 2);
    poz+=2;
    if (jeBroj(pom) && std::stoi(pom) < 60) this->prvi_polazak += std::stoi(pom) ;
        // Los unos - minuti moraju biti celobrojna vrednost, manja od 60
    else throw PogresnoVreme('M');

    if (linija_fajla[poz]=='-') poz++;
    else throw PogresanUnos(poz+1);

    pom = linija_fajla.substr(poz, 2);
    poz+=2;
    if (jeBroj(pom) && std::stoi(pom) < 24) this->poslednji_polazak = std::stoi(pom) * 60 ;
        // Los unos - satnica mora biti celobrojna vrednost, manja od 24
    else throw PogresnoVreme('S');

    if (linija_fajla[poz]==':') poz++;
    else throw PogresanUnos(poz+1);

    pom = linija_fajla.substr(poz, 2);
    poz+=2;
    if (jeBroj(pom) && std::stoi(pom) < 60) this->poslednji_polazak += std::stoi(pom) ;
        // Los unos - minuti moraju biti celobrojna vrednost, manja od 60
    else throw PogresnoVreme('M');

    if (linija_fajla[poz]=='#') poz++;
    else throw PogresanUnos(poz+1);

    pom.clear();
    if (linija_fajla.find(']') == std::string::npos) throw PogresanUnos();;
    while(linija_fajla[poz]!=']') pom+=linija_fajla[poz++];

    if (jeBroj(pom)) this->razmak_izmedju_polazaka = std::stoi(pom) ;
        // Los unos - minuti moraju biti celobrojna vrednost
    else throw PogresanUnos(poz+1);

    poz++; // preskace ']'

    if (linija_fajla[poz]==' ') poz++;
    else throw PogresanUnos(poz+1);

    // greska : (pocetak je posle kraja) - ako nema nocnih linija
    //if (this->prvi_polazak > this->poslednji_polazak) throw std::exception();
    if (this->prvi_polazak >= this->poslednji_polazak) this->poslednji_polazak += 24 * 60;

    this->broj_polazaka= (this->poslednji_polazak- this->prvi_polazak) / this->razmak_izmedju_polazaka + 1;

    int stajaliste;

    while (true){
        pom = procitajRec(linija_fajla, poz);

        if (jeBroj(pom)) stajaliste = std::stoi(pom) ;
        else throw PogresanUnos(poz+1);

        if (stajalista.count(stajaliste)) this->sifre_stajalista.push_back(stajaliste);
            // stajaliste ne postoji
        else throw ObjekatNijePronadjen('S');

        if (poz== linija_fajla.length() || linija_fajla[poz]=='\r') break;

        if (linija_fajla[poz]==' ') poz++;
        else throw PogresanUnos(poz+1);
    }
}

bool LinijaPrevoza::jeBroj(const std::string &s) {
    for (char const &c: s) if (!std::isdigit(c)) return false;
    return true;
}

std::string LinijaPrevoza::procitajRec(const std::string &linija, int &pozicija) {
    std::string rec;

    while (linija[pozicija]!=' ' && linija[pozicija]!='\n' && linija[pozicija]!='\r' && linija[pozicija]!='\0') {
        rec+=linija[pozicija++];
    }
    return rec;
}

bool LinijaPrevoza::prolaziKroz(int stajaliste) {
    if (std::count(this->sifre_stajalista.begin(), this->sifre_stajalista.end(), stajaliste)) return true;
    else return false;
}

std::string LinijaPrevoza::uzmiNaziv() {
    return this->oznaka_linije;
}

std::vector<int> LinijaPrevoza::uzmiSifreStajalista() {
    return this->sifre_stajalista;
}

int LinijaPrevoza::uzmiBrojPolazaka() const {
    return this->broj_polazaka;
}

int LinijaPrevoza::prethodno(int stajaliste) {

    auto i = std::find(this->sifre_stajalista.begin(), this->sifre_stajalista.end(), stajaliste);

    if (i != this->sifre_stajalista.end()) { // prolazi kroz stajaliste
        int indeks = i - this->sifre_stajalista.begin();
        if (indeks!=0) return this->sifre_stajalista[indeks-1];
    }
    return 0;
}

int LinijaPrevoza::sledece(int stajaliste) {
    auto i = std::find(this->sifre_stajalista.begin(), this->sifre_stajalista.end(), stajaliste);

    if (i != this->sifre_stajalista.end()) { // prolazi kroz stajaliste
        int indeks = i - this->sifre_stajalista.begin();
        if (indeks != this->sifre_stajalista.size()-1 ) return this->sifre_stajalista[indeks+1];
    }
    return 0;
}

int LinijaPrevoza::doSledecegProlaska(int stajaliste, int trenutno_vreme) {

    auto i = std::find(this->sifre_stajalista.begin(), this->sifre_stajalista.end(), stajaliste);

    if (i != this->sifre_stajalista.end()) { // prolazi kroz stajaliste
        int indeks = i - this->sifre_stajalista.begin();
        int vreme_polaska = this->prvi_polazak;
        // u slucaju nocne linije :
        if (this->poslednji_polazak >= 24*60 && vreme_polaska > trenutno_vreme && trenutno_vreme <= this->poslednji_polazak % (24*60)) trenutno_vreme += 24*60;
        while(vreme_polaska < trenutno_vreme - indeks * 3) vreme_polaska += this->razmak_izmedju_polazaka;
        if (vreme_polaska > this->poslednji_polazak) return INF;
        if (vreme_polaska + indeks * 3 - trenutno_vreme > MAX_CEKANJE) return INF;
        else return vreme_polaska + indeks * 3 - trenutno_vreme;
    }
    return INF;
}
