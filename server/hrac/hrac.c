//
// Created by jakub on 7. 1. 2024.
//
#include <stdio.h>
#include "hrac.h"
#include "stdlib.h"

void initializeHrac(Hrac* hrac, int idHraca, char farbaFigurky) {
    hrac->idHraca = idHraca;
    hrac->farbaFigurky = farbaFigurky;
}

void jePripraveny(Hrac* hrac) {
    printf("Hrac %d je pripraveny!\n", hrac->idHraca);
}

int getIdHraca(Hrac* hrac){
    return hrac->idHraca;
}

char getFarbaFigurky(const Hrac* hrac) {
    return hrac->farbaFigurky;
}

int hodKockou() {
    return rand() % 6 + 1;
}
