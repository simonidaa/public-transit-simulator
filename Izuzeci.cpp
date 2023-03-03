#include "Izuzeci.h"

std::string GreskaUFajlu::poruka(const std::string& ime_fajla) const {
    if (this->pozicija) {
        return "(Greska u fajlu) : " + ime_fajla + " : linija " + std::to_string(this->linija) + "; karakter : " + std::to_string(this->pozicija) + ".\n";
    }
    else return "Pogresan unos na liniji " + std::to_string(this->linija) + ".\n";
}


std::string GreskaUFajlu::poruka() {
    return "Pogresan unos na liniji " + std::to_string(this->linija) + ".\n";
}

std::string FajlNijePronadjen::poruka() {
    switch (this->ui) {
        case 'U':
            return "(Fajl nije pronadjen) : Otvaranje ulaznog fajla nije uspelo.\n";
        case 'I':
            return "(Fajl nije pronadjen) : Otvaranje izlaznog fajla nije uspelo.\n";
        default:
            return "(Fajl nije pronadjen) : Otvaranje fajla nije uspelo.\n";
    }
}


std::string OznakaNijeJedinstvena::poruka() {
    switch (this->sl) {
        case 'L':
            return "(Pogresan unos) : Oznaka linije nije jedinstvena.\n";
        case 'S':
            return "(Pogresan unos) : Sifra stajalista nije jedinstvena.\n";
        default:
            return "(Pogresan unos) : Sifra/oznaka nije jedinstvena.\n";
    }
}

std::string OznakaNijeJedinstvena::poruka(int linija) const {
    switch (this->sl) {
        case 'L':
            return "(Pogresan unos na liniji " + std::to_string(linija) + ") : Oznaka linije nije jedinstvena.\n";
        case 'S':
            return "(Pogresan unos na liniji " + std::to_string(linija) + ") : Sifra stajalista nije jedinstvena.\n";
        default:
            return "(Pogresan unos na liniji " + std::to_string(linija) + ") : Sifra/oznaka nije jedinstvena.\n";
    }
}

std::string PogresanUnos::poruka() {
    return "Pogresan unos! Molimo Vas, pokusajte ponovo.\n";
}

int PogresanUnos::uzmiPoziciju() const {
    return this->pozicija;
}

std::string PogresanUnos::poruka(int linija) const {
    if (this->pozicija) return "Pogresan unos : linija " + std::to_string(linija) + "; karakter : " + std::to_string(this->pozicija) + ".\n";
    else return "Pogresan unos na liniji " + std::to_string(linija) + ".\n";
}

std::string ObjekatNijePronadjen::poruka() {
    switch (this->sl) {
        case 'L':
            return "Linija sa datom oznakom nije pronadjena.\n";
        case 'S':
            return "Stajaliste sa datom sifrom nije pronadjeno.\n";
        default:
            return "Trazeni objekat nije pronadjen. Molimo Vas, proverite unos.\n";
    }
}

std::string PogresnoVreme::poruka() {
    switch (this->sm) {
        case 'S':
            return "(Pogresan unos vremena) : Satnica mora biti ceo broj manji od 24!\n";
        case 'M':
            return "(Pogresan unos vremena) : Minuti moraju biti ceo broj manji od 60!\n";
        default:
            return "Pogresan unos vremena. Molimo Vas, proverite unos.\n";
    }
}
