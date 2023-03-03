#include <iostream>
#include "MrezaGradskogPrevoza.h"

int main() {
    auto* mreza = new MrezaGradskogPrevoza();
    mreza->pokreniSimulator();
    delete mreza;
    /*mreza->ucitajStajalista("stajalista.txt");
    mreza->ucitajLinije("linije.txt");
    mreza->prikazStajalista(585);
    mreza->prikazLinije("12");
    mreza->pronadjiNajbrzuPutanju(123,99,"25:00");*/
}
