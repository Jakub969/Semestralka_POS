//
// Created by jakub on 7. 1. 2024.
//

#ifndef SERVER_HRAC_H
#define SERVER_HRAC_H

typedef struct {
    int idHraca;
    char farbaFigurky;
} Hrac;

int getIdHraca(Hrac* hrac);

char getFarbaHraca(Hrac* hrac);

#endif //SERVER_HRAC_H
