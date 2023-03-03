#include "Cvor.h"

void Cvor::resi() {
    this->resen = true;
}

bool Cvor::jeResen() const {
    return this->resen;
}

int Cvor::uzmiTrajanje() const {
    return this->trajanje;
}

int Cvor::uzmiPresedanja() const {
    return this->presedanja;
}

Cvor *Cvor::uzmiRoditelja() {
    return this->roditelj;
}

Tip Cvor::uzmiTip() {
    return this->tip;
}

int Cvor::uzmiSifru() const {
    return this->sifra;
}

void Cvor::postavi(int vreme, int trajanje_, Cvor *roditelj_) {
    this->trenutno_vreme = vreme;
    this->trajanje=trajanje_;
    this->roditelj=roditelj_;

    this->presedanja = roditelj_->uzmiPresedanja();
    if (this->tip==PERON && roditelj_->uzmiTip()==STANICA && roditelj_->uzmiRoditelja()!= nullptr) this->presedanja++;
}

void Stanica::postaviPrvi(int vreme) {
    this->trenutno_vreme=vreme;
    this->trajanje=0;
    this->presedanja=0;
    this->roditelj= nullptr;
}

Stanica::Stanica() {
    this->tip = STANICA;
}

void Stanica::ucitajStanicu(const int stajaliste, const std::map<int, std::string> &stajalista, const std::vector<LinijaPrevoza *> &linije, std::vector<Cvor*> &graf) {
    this->sifra=stajaliste;

    for(auto* linija : linije) {

        if (linija->prolaziKroz(stajaliste)) {
            auto* novi = new Peron(stajaliste, linija);
            graf.push_back(novi);
            susedi[novi]=INF;
        }
    }

}

std::map<Cvor *, int> Stanica::uzmiSusede() {

    for (auto& elem : susedi) {
        auto* cvor = dynamic_cast<Peron*>(elem.first);
        susedi[cvor] = cvor->uzmiLiniju()->doSledecegProlaska(this->sifra, this->trenutno_vreme);
    }

    return this->susedi;
}

Stanica::~Stanica() {
    for (auto& elem : susedi) {
        auto* cvor = dynamic_cast<Peron*>(elem.first);
        delete cvor;
    }
}


Peron::Peron(int stajaliste, LinijaPrevoza *linija_) {
    this->tip = PERON;
    this->linija = linija_;
    this->sifra = stajaliste;
}

void Peron::ucitajPeron(std::vector<Cvor*> &graf) {
    int sledeci = this->linija->sledece(this->sifra);
    //int prethodni = this->linija->prethodno(this->sifra);

    for(const auto& elem : graf){
        if (elem->uzmiSifru() == this->sifra && elem->uzmiTip() == STANICA) {
            susedi[elem]=0;
        }

        if (elem->uzmiSifru() == sledeci && elem->uzmiTip() == PERON) {
            if (dynamic_cast<Peron*>(elem)->uzmiLiniju() == this->linija) susedi[elem]=3;
        }

        /*if (elem->uzmiSifru() == prethodni && elem->uzmiTip() == PERON) {
            if (dynamic_cast<Peron*>(elem)->uzmiLiniju() == this->linija) susedi[elem]=3;
        }*/
    }
}

LinijaPrevoza *Peron::uzmiLiniju() {
    return this->linija;
}

std::map<Cvor *, int> Peron::uzmiSusede() {
    return this->susedi;
}

