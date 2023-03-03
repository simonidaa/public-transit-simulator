#ifndef OOP_PROJEKAT_CVOR_H
#define OOP_PROJEKAT_CVOR_H

#include "LinijaPrevoza.h"

#include <map>
#include <string>

enum Tip {
    STANICA, PERON
};

class Cvor {
public:
    Cvor()=default;
    virtual ~Cvor()=default;
    Cvor(const Cvor& cvor)=delete;
    Cvor& operator= (const Cvor& cvor) = delete;

    virtual std::map<Cvor*, int> uzmiSusede()=0;

    bool jeResen() const;
    int uzmiTrajanje() const;
    int uzmiPresedanja() const;
    Cvor* uzmiRoditelja();
    Tip uzmiTip();
    int uzmiSifru() const;

    void resi();

    void postavi(int vreme, int trajanje_, Cvor* roditelj);
protected:
    int trajanje = INF;
    int presedanja = INF;
    bool resen = false;
    int trenutno_vreme;
    Tip tip;
    Cvor* roditelj;
    int sifra;

    std::map<Cvor*, int> susedi;

};

class Stanica : public Cvor {
public:
    Stanica();
    Stanica(const Stanica& stanica)=delete;
    ~Stanica() override;
    Stanica& operator= (const Stanica& stanica) = delete;

    std::map<Cvor*, int> uzmiSusede() override;

    void postaviPrvi(int vreme);
    void ucitajStanicu(int stajaliste, const std::map<int, std::string> &stajalista, const std::vector<LinijaPrevoza*> &linije, std::vector<Cvor *> &graf);
protected:
};


class Peron : public Cvor {
public:
    Peron(int stajaliste, LinijaPrevoza* linija_);
    Peron(const Peron& peron)=delete;
    Peron& operator= (const Peron& peron) = delete;

    std::map<Cvor*, int> uzmiSusede() override;

    void ucitajPeron(std::vector<Cvor*> &graf);
    LinijaPrevoza* uzmiLiniju();

protected:
    LinijaPrevoza* linija;
};


#endif //OOP_PROJEKAT_CVOR_H
