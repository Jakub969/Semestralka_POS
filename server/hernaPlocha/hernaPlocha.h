//
// Created by jakub on 7. 1. 2024.
//

#ifndef CLIENT_HERNAPLOCHA_H
#define CLIENT_HERNAPLOCHA_H

#include <stdlib.h>
#include <stdbool.h>
#define VELKOST_MAPY 11

typedef struct {
    int r;
    int s;
} Suradnica;

typedef struct {
    char** pole;
    int velkostMapy;
    int pocetHracov;
    int hracNaRade;
} HernaPlocha;

void initializeSuradnica(Suradnica* suradnica, int r, int s);
void initializeHernaPlocha(HernaPlocha* hernaplocha, char** parametre);
void freeHernaPlocha(HernaPlocha* hernaPlocha);
void vypisSa(const HernaPlocha* hernaplocha);
void aktualizujSa(HernaPlocha* hernaplocha, char** parametre);
void alokujPole(HernaPlocha* hernaplocha);
char* getFarbuHracaNaRade(const HernaPlocha* hernaplocha);
Suradnica getSuradnicuPodlaIndexu(const HernaPlocha* hernaplocha, int index);
void nastavDefaultnePolicka(HernaPlocha* hernaplocha);
Suradnica* getSlovnik(const HernaPlocha* hernaplocha);
char getZnakPodlaCislaHraca(const HernaPlocha* hernaplocha, int cisloHraca);
Suradnica* getZapornySlovnik(const HernaPlocha* hernaplocha);

#endif //CLIENT_HERNAPLOCHA_H
