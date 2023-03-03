#include "MrezaGradskogPrevoza.h"

void MrezaGradskogPrevoza::ucitajStajalista(const std::string &putanja_fajla) {
    std::ifstream ulaz(putanja_fajla);

    std::string linija;
    int redni_broj_linije = 0;

    if (ulaz.is_open()) {
        // ucitava liniju po liniju
        while (std::getline(ulaz, linija)) {
            redni_broj_linije++;
            try {
                ucitajStajaliste(linija);
            }
            catch (PogresanUnos &izuzetak) {
                // ako ucitava ostale linije kada naidje na gresku
                std::cout << izuzetak.poruka(redni_broj_linije) << "Ostale linije bice ucitane.\n";

                // ako prekida ucitavanje kada naidje na gresku
                //throw GreskaUFajlu(redni_broj_linije, izuzetak.uzmiPoziciju());
            }
            catch (OznakaNijeJedinstvena &izuzetak) {
                // ako ucitava ostale linije kada naidje na gresku
                std::cout << izuzetak.poruka(redni_broj_linije) << "Ostale linije bice ucitane.\n";

                // ako prekida ucitavanje kada naidje na gresku
                //throw GreskaUFajlu(redni_broj_linije);
            }
        }

        ulaz.close();
    }

    else throw FajlNijePronadjen('U');

}

void MrezaGradskogPrevoza::ucitajLinije(const std::string &putanja_fajla) {
    std::ifstream ulaz(putanja_fajla);

    std::string linija;
    int redni_broj_linije = 0;

    if (ulaz.is_open()) {
        // ucitava liniju po liniju
        while (std::getline(ulaz, linija)) {
            auto* novaLinija = new LinijaPrevoza();

            try {
                redni_broj_linije++;
                novaLinija->ucitajLiniju(linija, this->stajalista);

                // provera da li je oznaka jedinstvena
                for(const auto& elem : linije){
                    if (elem->uzmiNaziv() == novaLinija->uzmiNaziv()) throw OznakaNijeJedinstvena('L');
                }

                this->linije.push_back(novaLinija);
            }
            catch (PogresanUnos &izuzetak) {
                delete novaLinija;
                // ako ucitava ostale linije kada naidje na gresku
                std::cout << izuzetak.poruka(redni_broj_linije) << "Ostale linije bice ucitane.\n";

                // ako prekida ucitavanje kada naidje na gresku
                //throw GreskaUFajlu(redni_broj_linije, izuzetak.uzmiPoziciju());
            }

            catch (PogresnoVreme &izuzetak) {
                std::cout << "Pogresan unos na liniji " + std::to_string(redni_broj_linije) + " : " << izuzetak.poruka();
                std::cout << "Ostale linije bice ucitane.\n";

                // ako prekida ucitavanje kada naidje na gresku
                //throw GreskaUFajlu(redni_broj_linije);
            }

            catch (ObjekatNijePronadjen &izuzetak) {
                // ako linija ima stanicu koja ne postoji u sistemu
                std::cout << "(Pogresan unos na liniji " + std::to_string(redni_broj_linije) + ") : " << izuzetak.poruka();
                std::cout << "Ostale linije bice ucitane.\n";

                // ako prekida ucitavanje kada naidje na gresku
                //throw GreskaUFajlu(redni_broj_linije);
            }

            catch (OznakaNijeJedinstvena &izuzetak) {
                std::cout << izuzetak.poruka(redni_broj_linije) << "Ostale linije bice ucitane.\n";

                // ako prekida ucitavanje kada naidje na gresku
                //throw GreskaUFajlu(redni_broj_linije);
            }
        }

        ulaz.close();
    }

    else throw FajlNijePronadjen('U');

}

void MrezaGradskogPrevoza::ucitajStajaliste(const std::string &linija_fajla) {
    // linija fajla je u formatu : sifra_stajališta naziv_stajalista

    int sifra;
    std::string naziv;

    int poz = 0;
    std::string sifra_stajalista = procitajRec(linija_fajla, poz);

    if (jeBroj(sifra_stajalista)) sifra = std::stoi(sifra_stajalista) ;
        // Los unos - sifra stajalista mora biti celobrojna vrednost
    else throw PogresanUnos();

    // provera da li je stajaliste jedinstveno
    if (this->stajalista.count(sifra)) throw OznakaNijeJedinstvena('S');

    if (linija_fajla[poz]==' ') poz++;
    else throw PogresanUnos(poz+1);

    while (linija_fajla[poz]!='\n' && linija_fajla[poz]!='\r' && linija_fajla[poz]!='\0') {
        naziv+=linija_fajla[poz++];
    }

    this->stajalista[sifra]=naziv;

}

std::string MrezaGradskogPrevoza::procitajRec(const std::string &linija, int &pozicija) {
    std::string rec;

    while (linija[pozicija]!=' ' && linija[pozicija]!='\n' && linija[pozicija]!='\r' && linija[pozicija]!='\0') {
        rec+=linija[pozicija++];
    }
    return rec;
}

bool MrezaGradskogPrevoza::jeBroj(const std::string &s) {
    for (char const &c: s) if (!std::isdigit(c)) return false;
    return true;
}

void MrezaGradskogPrevoza::prikazStajalista(int stajaliste) {
    if (this->stajalista.count(stajaliste)) {

        std::vector<std::string> linije_kroz;

        for(auto* elem : this->linije){
            if (elem->prolaziKroz(stajaliste)) linije_kroz.push_back(elem->uzmiNaziv());
        }

        std::sort(linije_kroz.begin(), linije_kroz.end());

        std::string ispis;
        ispis += std::to_string(stajaliste) + ' ' + this->stajalista[stajaliste] + " [";

        for (auto &linija : linije_kroz){
            ispis += linija;
            if (linija!=linije_kroz.back()) ispis += ' ';
        }
        ispis += ']';

        std::string ime_fajla = "stajaliste_" + std::to_string(stajaliste) + ".txt";
        izlazniFajl(ime_fajla, ispis);
        std::cout << "Generisan je fajl " + ime_fajla + " sa osnovnim informacijama o stajalistu " + std::to_string(stajaliste) + ".\n";

    }
    else std::cout << "Stajaliste sa datom sifrom nije pronadjeno.\n";
}

void MrezaGradskogPrevoza::prikazLinije(const std::string& oznaka_linije) {

    LinijaPrevoza* linija;

    try {
        linija = uzmiLiniju(oznaka_linije);
    }
    catch (ObjekatNijePronadjen &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }

    std::vector<int> sifre_stajalista = linija->uzmiSifreStajalista();

    std::string ispis;
    ispis += linija->uzmiNaziv() + ' ';
    ispis += this->stajalista[sifre_stajalista.front()] + "->" + this->stajalista[sifre_stajalista.back()] + '\n';

    for(auto sifra : sifre_stajalista){
        ispis += std::to_string(sifra) + ' ' + this->stajalista[sifra] + '\n';
    }
    ispis.pop_back();

    std::string ime_fajla = "linija_" + oznaka_linije + ".txt";
    izlazniFajl(ime_fajla, ispis);
    std::cout << "Generisan je fajl " + ime_fajla + " sa osnovnim informacijama o liniji " + oznaka_linije + ".\n";

}

LinijaPrevoza *MrezaGradskogPrevoza::uzmiLiniju(const std::string& oznaka_linije) {
    for(auto* elem : this->linije){
        if (elem->uzmiNaziv() == oznaka_linije) return elem;
    }

    throw ObjekatNijePronadjen('L');
}

void MrezaGradskogPrevoza::prikazStatistikeLinije(const std::string& oznaka_linije) {

    LinijaPrevoza* linija;

    try {
        linija = uzmiLiniju(oznaka_linije);
    }
    catch (ObjekatNijePronadjen &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }

    std::vector<std::string> sa_zajednickim_stajalistem;

    for(auto* elem : this->linije){
        if (imajuZajednicko(linija, elem) && linija->uzmiNaziv()!=elem->uzmiNaziv()) sa_zajednickim_stajalistem.push_back(elem->uzmiNaziv());
    }

    std::sort(sa_zajednickim_stajalistem.begin(), sa_zajednickim_stajalistem.end());

    std::string ispis;
    ispis += linija->uzmiNaziv() + '\n';
    for (auto &oznaka : sa_zajednickim_stajalistem){
        ispis += oznaka;
        if (oznaka!=sa_zajednickim_stajalistem.back()) ispis += ' ';
        else ispis+='\n';
    }
    ispis += std::to_string(linija->uzmiBrojPolazaka());

    std::string ime_fajla = "statistika_" + oznaka_linije + ".txt";
    izlazniFajl(ime_fajla, ispis);
    std::cout << "Generisan je fajl " + ime_fajla + " sa statistickim informacijama o liniji " + oznaka_linije + ".\n";

}

bool MrezaGradskogPrevoza::imajuZajednicko(LinijaPrevoza *prva, LinijaPrevoza *druga) {
    std::vector<int> stajalista_prve = prva->uzmiSifreStajalista();

    for(auto sifra : stajalista_prve){
        if(druga->prolaziKroz(sifra)) return true;
    }
    return false;
}

int MrezaGradskogPrevoza::procitajVreme(const std::string &vreme) {
    int minuti;

    int poz = 0;
    std::string pom;

    pom = vreme.substr(poz, 2);
    poz+=2;

    if (jeBroj(pom) && std::stoi(pom) < 24) minuti = std::stoi(pom) * 60 ;
        // Los unos - satnica mora biti celobrojna vrednost, manja od 24
    else throw PogresnoVreme('S');

    if (vreme[poz]==':') poz++;
    else throw PogresanUnos(poz+1); //Los unos

    pom = vreme.substr(poz, 2);
    poz+=2;
    if (jeBroj(pom) && std::stoi(pom) < 60) minuti += std::stoi(pom) ;
        // Los unos - minuti moraju biti celobrojna vrednost, manja od 60
    else throw PogresnoVreme('M');

    if (vreme[poz]!='\0') throw PogresanUnos(poz);

    return minuti;
}

void MrezaGradskogPrevoza::pronadjiNajbrzuPutanju(int pocetak, int kraj, const std::string &vreme) {

    int trenutno_vreme;
    Cvor* pocetni;
    Cvor* krajnji;

    std::vector<Cvor *> graf = generisiGraf();

    try{
        pocetni = pronadjiStanicu(pocetak, graf);
        krajnji = pronadjiStanicu(kraj, graf);
        trenutno_vreme = procitajVreme(vreme);
    }
    catch (ObjekatNijePronadjen &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }
    catch (PogresnoVreme &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }

    dynamic_cast<Stanica*>(pocetni)->postaviPrvi(trenutno_vreme);

    int broj_resenih=0;
    while(broj_resenih < graf.size()) {

        Cvor* najblizi_cvor = pocetni;
        int najkrace_vreme_trajanja = INF;

        for (const auto& cvor : graf) {
            if (!cvor->jeResen() && cvor->uzmiTrajanje() < najkrace_vreme_trajanja) {
                najblizi_cvor = cvor;
                najkrace_vreme_trajanja = cvor->uzmiTrajanje();
            }
        }

        najblizi_cvor->resi();
        broj_resenih++;

        if(najblizi_cvor==krajnji) break;

        // azurira susede ukoliko je potrebno
        std::map<Cvor*, int> susedi = najblizi_cvor->uzmiSusede();

        for (const auto& [cvor, rastojanje] : susedi) {

            if (!cvor->jeResen() && najblizi_cvor->uzmiTrajanje() + rastojanje < cvor->uzmiTrajanje()) {
                cvor->postavi(trenutno_vreme + najblizi_cvor->uzmiTrajanje() + rastojanje, najblizi_cvor->uzmiTrajanje() + rastojanje, najblizi_cvor);
            }

        }

    }

    std::string ime_fajla = "putanja_" + std::to_string(pocetak) + '_' + std::to_string(kraj) + ".txt";
    std::string poruka = "Put pronadjen! Trajanje : " + std::to_string(krajnji->uzmiTrajanje()) + " minuta; Broj presedanja : " + std::to_string(krajnji->uzmiPresedanja()) + ".\n";
    poruka += "Generisan je fajl " + ime_fajla + " sa simulacijom kretanja kroz mrezu gradskog prevoza od stajalista ";
    poruka += std::to_string(pocetak) + " do stajalista " + std::to_string(kraj) + " najbrzom putanjom u " + vreme + ".\n";
    ispisPutanje(krajnji, ime_fajla, poruka);

    for(const auto& elem : graf){
        if (elem->uzmiTip() == STANICA) {
            delete elem;
        }
    }
}

std::vector<Cvor *> MrezaGradskogPrevoza::generisiGraf() {
    std::vector<Cvor *> graf;

    for(const auto& elem : this->stajalista)
    {
        auto* nova = new Stanica();
        nova->ucitajStanicu(elem.first, this->stajalista, this->linije, graf);
        graf.push_back(nova);
    }

    for(const auto& elem : graf){
        if (elem->uzmiTip() == PERON) {
            dynamic_cast<Peron*>(elem)->ucitajPeron(graf);
        }
    }

    return graf;
}

void MrezaGradskogPrevoza::pronadjiNajmanjePresedanja(int pocetak, int kraj, const std::string &vreme) {
    int trenutno_vreme;
    Cvor* pocetni;
    Cvor* krajnji;

    std::vector<Cvor *> graf = generisiGraf();

    try{
        pocetni = pronadjiStanicu(pocetak, graf);
        krajnji = pronadjiStanicu(kraj, graf);
        trenutno_vreme = procitajVreme(vreme);
    }
    catch (ObjekatNijePronadjen &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }
    catch (PogresnoVreme &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }

    dynamic_cast<Stanica*>(pocetni)->postaviPrvi(trenutno_vreme);

    int broj_resenih=0;
    while(broj_resenih < graf.size()) {

        Cvor* najblizi_cvor = pocetni;
        int najmanji_broj_presedanja = INF;

        for (const auto& cvor : graf) {
            if (!cvor->jeResen() && cvor->uzmiTrajanje() < INF && cvor->uzmiPresedanja() < najmanji_broj_presedanja) {
                najblizi_cvor = cvor;
                najmanji_broj_presedanja = cvor->uzmiPresedanja();
            }
        }

        najblizi_cvor->resi();
        broj_resenih++;

        if(najblizi_cvor==krajnji) break;

        // azurira susede ukoliko je potrebno
        std::map<Cvor*, int> susedi = najblizi_cvor->uzmiSusede();

        for (const auto& [cvor, rastojanje] : susedi) {

            int nova_presedanja = najblizi_cvor->uzmiPresedanja();
            if(najblizi_cvor->uzmiTip()==STANICA && cvor->uzmiTip()==PERON && najblizi_cvor->uzmiRoditelja()!= nullptr) nova_presedanja++;

            if (!cvor->jeResen() && (nova_presedanja < cvor->uzmiPresedanja() )) {
                cvor->postavi(trenutno_vreme + najblizi_cvor->uzmiTrajanje() + rastojanje, najblizi_cvor->uzmiTrajanje() + rastojanje, najblizi_cvor);
            }

        }

    }


    std::string ime_fajla = "putanja_" + std::to_string(pocetak) + '_' + std::to_string(kraj) + ".txt";
    std::string poruka = "Put pronadjen! Trajanje : " + std::to_string(krajnji->uzmiTrajanje()) + " minuta; Broj presedanja : " + std::to_string(krajnji->uzmiPresedanja()) + ".\n";
    poruka += "Generisan je fajl " + ime_fajla + " sa simulacijom kretanja kroz mrezu gradskog prevoza od stajalista ";
    poruka += std::to_string(pocetak) + " do stajalista " + std::to_string(kraj) + " putanjom sa najmanjim brojem presedanja u " + vreme + ".\n";
    ispisPutanje(krajnji, ime_fajla, poruka);

    for(const auto& elem : graf){
        if (elem->uzmiTip() == STANICA) {
            delete elem;
        }
    }
}

void MrezaGradskogPrevoza::pronadjiPutanju(int pocetak, int kraj, const std::string &vreme) {
    int trenutno_vreme;
    Cvor* pocetni;
    Cvor* krajnji;

    std::vector<Cvor *> graf = generisiGraf();

    try{
        pocetni = pronadjiStanicu(pocetak, graf);
        krajnji = pronadjiStanicu(kraj, graf);
        trenutno_vreme = procitajVreme(vreme);
    }
    catch (ObjekatNijePronadjen &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }
    catch (PogresnoVreme &izuzetak) {
        std::cout << izuzetak.poruka();
        return;
    }

    dynamic_cast<Stanica*>(pocetni)->postaviPrvi(trenutno_vreme);

    int broj_resenih=0;
    while(broj_resenih < graf.size()) {

        if ( krajnji->uzmiTrajanje() < INF) break;

        Cvor* najblizi_cvor = pocetni;
        int najkrace_vreme_trajanja = INF;

        for (const auto& cvor : graf) {
            if (!cvor->jeResen() && cvor->uzmiTrajanje() < najkrace_vreme_trajanja) {
                najblizi_cvor = cvor;
                najkrace_vreme_trajanja = cvor->uzmiTrajanje();
            }
        }

        najblizi_cvor->resi();
        broj_resenih++;

        if(najblizi_cvor==krajnji) break;

        // azurira susede ukoliko je potrebno
        std::map<Cvor*, int> susedi = najblizi_cvor->uzmiSusede();

        for (const auto& [cvor, rastojanje] : susedi) {

            if (!cvor->jeResen() && najblizi_cvor->uzmiTrajanje() + rastojanje < cvor->uzmiTrajanje()) {
                cvor->postavi(trenutno_vreme + najblizi_cvor->uzmiTrajanje() + rastojanje, najblizi_cvor->uzmiTrajanje() + rastojanje, najblizi_cvor);
            }

        }

    }


    std::string ime_fajla = "putanja_" + std::to_string(pocetak) + '_' + std::to_string(kraj) + ".txt";
    std::string poruka = "Put pronadjen! Trajanje : " + std::to_string(krajnji->uzmiTrajanje()) + " minuta; Broj presedanja : " + std::to_string(krajnji->uzmiPresedanja()) + ".\n";
    poruka += "Generisan je fajl " + ime_fajla + " sa simulacijom kretanja kroz mrezu gradskog prevoza od stajalista ";
    poruka += std::to_string(pocetak) + " do stajalista " + std::to_string(kraj) + " u " + vreme + ".\n";
    ispisPutanje(krajnji, ime_fajla, poruka);

    for(const auto& elem : graf){
        if (elem->uzmiTip() == STANICA) {
            delete elem;
        }
    }
}

void MrezaGradskogPrevoza::pokreniSimulator() {
    std::string naslov = "                                                                       _____________\n"
                         "┌┬┐┬─┐┌─┐┌─┐┌─┐  ┌─┐┬─┐┌─┐┌┬┐┌─┐┬┌─┌─┐┌─┐  ┌─┐┬─┐┌─┐┬  ┬┌─┐┌─┐┌─┐   _/_|[][][][][] | - -\n"
                         "│││├┬┘├┤ ┌─┘├─┤  │ ┬├┬┘├─┤ ││└─┐├┴┐│ ││ ┬  ├─┘├┬┘├┤ └┐┌┘│ │┌─┘├─┤  (      Beograd  | - -\n"
                         "┴ ┴┴└─└─┘└─┘┴ ┴  └─┘┴└─┴ ┴─┴┘└─┘┴ ┴└─┘└─┘  ┴  ┴└─└─┘ └┘ └─┘└─┘┴ ┴  =--OO-------OO--= - -\n\n";

    int opcija = -1;
    std::cout << naslov;

    try{
        std::string putanja_fajla;

        while(opcija != 0){
            this->stajalista.clear();
            this->linije.clear();
            std::cout << "Dobrodošli u simulator mreže gradskog prevoza. Molimo Vas, odaberite opciju: \n";
            std::cout << "1. Učitavanje podataka o mreži gradskog prevoza\n"
                         "0. Kraj rada\n";

            try{
                opcija = procitajBroj();
            }
            catch (PogresanUnos &izuzetak) {
                opcija = -1;
            }

            switch (opcija) {
                case 0:
                    break;
                case 1:
                    try{
                        std::cout << "Molimo Vas, unesite putanju do fajla sa stajalištima:\n";
                        std::cin >> putanja_fajla;
                        this->ucitajStajalista(putanja_fajla);
                        std::cout << "Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:\n";
                        std::cin >> putanja_fajla;
                        this->ucitajLinije(putanja_fajla);

                        std::cout << "Mreža gradskog prevoza je uspešno učitana. ";
                        this->pokreniOpcije();
                        opcija = 0;
                        break;
                    }
                    catch (FajlNijePronadjen &izuzetak) {
                        std::cout << izuzetak.poruka() << "Molimo Vas, pokusajte ponovo.\n";
                        continue;
                    }
                    catch (GreskaUFajlu &izuzetak){
                        std::cout << izuzetak.poruka(putanja_fajla) << "Molimo Vas, pokusajte ponovo.\n";
                        continue;
                    }
                default:
                    std::cout << "Opcija ne postoji! Molimo Vas, pokusajte ponovo.\n";
            }
        }
    }

    catch (...) {
        std::cout << "Doslo je do nepredvidjene greske. Molimo Vas, pokusajte ponovo.\n";
        return;
    }

}

void MrezaGradskogPrevoza::pokreniOpcije() {
    int opcija = -1;

    int stajaliste;

    std::string linija;

    int pocetak, kraj;
    std::string vreme;

    while (opcija!=0) {
        std::cout << "Molimo Vas, odaberite opciju: \n"
                     "1. Prikaz informacija o stajalištu\n"
                     "2. Prikaz osnovnih informacija o liniji gradskog prevoza\n"
                     "3. Prikaz statističkih informacija o liniji gradskog prevoza\n"
                     "4. Pronalazak putanje između dva stajališta\n"
                     "0. Kraj rada\n";

        try{
            opcija = procitajBroj();
        }
        catch (PogresanUnos &izuzetak) {
            opcija = -1;
        }

        switch (opcija) {
            case 0:
                break;
            case 1:
                try{
                    stajaliste = procitajBroj();
                    this->prikazStajalista(stajaliste);
                }
                catch (PogresanUnos& izuzetak) {
                    std::cout << izuzetak.poruka();
                    break;
                }
                break;
            case 2:
                std::cin >> linija;
                this->prikazLinije(linija);
                break;
            case 3:
                std::cin >> linija;
                this->prikazStatistikeLinije(linija);
                break;
            case 4:
                std::cout << "Molimo Vas, odaberite opciju: \n"
                             "1. Pronalazak bilo koje putanje između dva stajališta\n"
                             "2. Pronalazak najbrže putanje između dva stajališta\n"
                             "3. Pronalazak putanje između dva stajališta sa najmanjim brojem presedanja\n"
                             "0. Kraj rada\n";

                try{
                    opcija = procitajBroj();
                }
                catch (PogresanUnos &izuzetak) {
                    opcija = -1;
                }

                switch (opcija) {
                    case 0:
                        break;
                    case 1:
                        pocetak = procitajBroj(); kraj = procitajBroj();
                        std::cin >> vreme;
                        this->pronadjiPutanju(pocetak,kraj,vreme);
                        break;
                    case 2:
                        pocetak = procitajBroj(); kraj = procitajBroj();
                        std::cin >> vreme;
                        this->pronadjiNajbrzuPutanju(pocetak,kraj,vreme);
                        break;
                    case 3:
                        pocetak = procitajBroj(); kraj = procitajBroj();
                        std::cin >> vreme;
                        this->pronadjiNajmanjePresedanja(pocetak, kraj, vreme);
                        break;
                    default:
                        std::cout << "Opcija ne postoji! Molimo Vas, pokusajte ponovo.\n";
                }
                break;
            default:
                std::cout << "Opcija ne postoji! Molimo Vas, pokusajte ponovo.\n";
        }

    }
}

void MrezaGradskogPrevoza::ispisPutanje(Cvor *krajnji, const std::string& ime_fajla, const std::string &poruka) {
    std::string ispis;

    if (krajnji->uzmiTrajanje() == 0) {
        std::cout << "Vec ste na cilju!\n";
        return;
    }

    if (krajnji->uzmiTrajanje() < INF) {
        std::cout << poruka << '\n';
        std::stack<Cvor*> put;
        while (krajnji->uzmiRoditelja()) {
            krajnji = krajnji->uzmiRoditelja();
            if (krajnji->uzmiTip()==PERON) put.push(krajnji);
        }
        std::string prosla_linija;
        while(!put.empty()) {
            if (put.top()->uzmiRoditelja()->uzmiTip()==STANICA) {
                std::string trenutna_linija = dynamic_cast<Peron*>(put.top())->uzmiLiniju()->uzmiNaziv();
                ispis += '\n' + prosla_linija + "->" + trenutna_linija + '\n';
                prosla_linija = trenutna_linija;
            }
            ispis += std::to_string(put.top()->uzmiSifru()) + ' ';
            put.pop();
        }

        izlazniFajl(ime_fajla, ispis.substr(1));
    }
    else std::cout<<"Put nije pronadjen!\n";
}

void MrezaGradskogPrevoza::izlazniFajl(const std::string &ime_fajla, const std::string &ispis) {
    std::ofstream izlaz(ime_fajla);

    if (izlaz.is_open()) {
        izlaz << ispis;
        izlaz.close();
    }
    else {
        throw FajlNijePronadjen('I');
    }
}

int MrezaGradskogPrevoza::procitajBroj() {
    std::string ulaz;
    std::cin >> ulaz;
    if (jeBroj(ulaz)) return stoi(ulaz);
    else throw PogresanUnos();
}

Cvor *MrezaGradskogPrevoza::pronadjiStanicu(int sifra, std::vector<Cvor *> &graf) {

    for(const auto& elem : graf){
        if (elem->uzmiTip() == STANICA && elem->uzmiSifru()==sifra) {
            return elem;
        }
    }
    throw ObjekatNijePronadjen('S');
}

MrezaGradskogPrevoza::~MrezaGradskogPrevoza() {
    for(const auto& elem : this->linije){
        delete elem;
    }
}

