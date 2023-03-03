#ifndef OOP_PROJEKAT_IZUZECI_H
#define OOP_PROJEKAT_IZUZECI_H

#include <exception>
#include <string>

class Greska : public std::exception
{
public:
    Greska() : std::exception() {}
    virtual std::string poruka() = 0;
};

class FajlNijePronadjen : public Greska
{
public:
    explicit FajlNijePronadjen(char ui_) : Greska() , ui(ui_){}
    std::string poruka() override;

private:
    char ui;
};

class GreskaUFajlu : public Greska
{
public:
    explicit GreskaUFajlu(int linija_) : Greska() , linija(linija_){}
    explicit GreskaUFajlu(int linija_, int poz) : Greska() , linija(linija_), pozicija(poz) {}

    std::string poruka() override;
    std::string poruka(const std::string& ime_fajla) const;

private:
    int linija;
    int pozicija=0;
};

class OznakaNijeJedinstvena : public Greska
{
public:
    explicit OznakaNijeJedinstvena(char sl_) : Greska() , sl(sl_){}
    std::string poruka() override;
    std::string poruka(int linija) const;

private:
    char sl;
};

class ObjekatNijePronadjen : public Greska
{
public:
    explicit ObjekatNijePronadjen(char sl_) : Greska() , sl(sl_){}
    std::string poruka() override;

private:
    char sl;
};

class PogresnoVreme : public Greska
{
public:
    explicit PogresnoVreme(char sm_) : Greska() , sm(sm_){}
    std::string poruka() override;

private:
    char sm;
};

class PogresanUnos : public Greska
{
public:
    PogresanUnos() : Greska() {}
    explicit PogresanUnos(int poz) : Greska(), pozicija(poz) {}
    std::string poruka() override;
    std::string poruka(int linija) const;

    int uzmiPoziciju() const;

private:
    int pozicija=0;
};

#endif //OOP_PROJEKAT_IZUZECI_H
