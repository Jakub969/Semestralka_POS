//
// Created by jakub on 7. 1. 2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "hernaPlocha.h"

void initializeHernaPlocha(HernaPlocha* hernaplocha, char** parametre) {
    alokujPole(hernaplocha);
    hernaplocha->pocetHracov = atoi(parametre[1]);
    hernaplocha->hracNaRade = atoi(parametre[2]);
    aktualizujSa(hernaplocha, parametre);
}

void aktualizujSa(HernaPlocha* hernaplocha, char** parametre) {

    nastavDefaultnePolicka(hernaplocha);

    hernaplocha->pocetHracov = atoi(parametre[1]);
    hernaplocha->hracNaRade = atoi(parametre[2]);
    for (int hracI = 0; hracI < hernaplocha->pocetHracov; hracI++) {
        for (int i = 0; i < 4; i++) {
            int indexPolicka = atoi(parametre[3 + (hracI * 4) + i]);
            Suradnica sur = getSuradnicuPodlaIndexu(hernaplocha, indexPolicka);
            hernaplocha->pole[sur.r][sur.s] = getZnakPodlaCislaHraca(hernaplocha, hracI);
        }
    }

}

void nastavDefaultnePolicka(HernaPlocha* hernaplocha) {
    // hviezdičky:
    char znakPolicka = '*';
    char znakVyhernehoDomceku = '.';
    char znakDomceka = '+';

    for (int i = 0; i < 11; ++i) {
        hernaplocha->pole[4][i] = znakPolicka;
        hernaplocha->pole[6][i] = znakPolicka;

        hernaplocha->pole[i][4] = znakPolicka;
        hernaplocha->pole[i][6] = znakPolicka;
    }

    hernaplocha->pole[5][0] = znakPolicka;
    hernaplocha->pole[5][10] = znakPolicka;
    hernaplocha->pole[0][5] = znakPolicka;
    hernaplocha->pole[10][5] = znakPolicka;

    // výherné domčeky:
    for(int i = 0; i < 4; i++) {
        hernaplocha->pole[5][i + 1] = znakVyhernehoDomceku;
        hernaplocha->pole[5][i + 6] = znakVyhernehoDomceku;
        hernaplocha->pole[i + 1][5] = znakVyhernehoDomceku;
        hernaplocha->pole[i + 6][5] = znakVyhernehoDomceku;
    }

    // domčeky:
    hernaplocha->pole[0][0] = znakDomceka;
    hernaplocha->pole[0][1] = znakDomceka;
    hernaplocha->pole[1][0] = znakDomceka;
    hernaplocha->pole[1][1] = znakDomceka;

    hernaplocha->pole[0][9] = znakDomceka;
    hernaplocha->pole[0][10] = znakDomceka;
    hernaplocha->pole[1][9] = znakDomceka;
    hernaplocha->pole[1][10] = znakDomceka;

    hernaplocha->pole[9][0] = znakDomceka;
    hernaplocha->pole[9][1] = znakDomceka;
    hernaplocha->pole[10][0] = znakDomceka;
    hernaplocha->pole[10][1] = znakDomceka;

    hernaplocha->pole[9][9] = znakDomceka;
    hernaplocha->pole[9][10] = znakDomceka;
    hernaplocha->pole[10][9] = znakDomceka;
    hernaplocha->pole[10][10] = znakDomceka;
}

void freeHernaPlocha(HernaPlocha* hernaplocha) {
    for (int r = 0; r < hernaplocha->velkostMapy; r++) {
        for (int s = 0; s < hernaplocha->velkostMapy; s++) {
            hernaplocha->pole[r][s] = 0;
        }
        free(hernaplocha->pole[r]);
    }
    free(hernaplocha->pole);
}

void vypisSa(const HernaPlocha* hernaplocha) {
    printf("Pocet hracov: %d\n", hernaplocha->pocetHracov);
    printf("Na tahu je hrac cislo %d s farbou %s\n", hernaplocha->hracNaRade, getFarbuHracaNaRade(hernaplocha));
    for (int r = 0; r < hernaplocha->velkostMapy; ++r) {
        for (int s = 0; s < hernaplocha->velkostMapy; ++s) {
            printf("%c ", hernaplocha->pole[r][s]);
        }
        printf("\n");
    }
}

void alokujPole(HernaPlocha* hernaplocha) {
    hernaplocha->velkostMapy = VELKOST_MAPY;
    hernaplocha->pole = (char**) calloc(hernaplocha->velkostMapy, sizeof(char*));
    for (int r = 0; r < hernaplocha->velkostMapy; r++) {
        hernaplocha->pole[r] = (char*) calloc(hernaplocha->velkostMapy, sizeof(char));
        for (int s = 0; s < hernaplocha->velkostMapy; s++) {
            hernaplocha->pole[r][s]= ' ';
        }
    }
}

char* getFarbuHracaNaRade(const HernaPlocha* hernaplocha) {
    switch(hernaplocha->hracNaRade) {
        case 1: return "Modry";
        case 2: return "Cerveny";
        case 3: return "Zeleny";
        case 4: return "Oranzovy";
    }
    return "CHYBA neplatne cislo hraca na rade!!!!";
}

Suradnica getSuradnicuPodlaIndexu(const HernaPlocha* hernaplocha, int index) {
    Suradnica* slovnik = getSlovnik(hernaplocha);
    Suradnica* slovnikZaporny = getZapornySlovnik(hernaplocha);

    if (index > 0) {
        Suradnica odpoved = slovnik[index];
        free(slovnik);
        free(slovnikZaporny);
        return odpoved;
    } else {
        Suradnica odpoved = slovnikZaporny[abs(index)];
        free(slovnikZaporny);
        free(slovnik);
        return odpoved;
    }
}

Suradnica* getSlovnik(const HernaPlocha* hernaplocha) {
    Suradnica* slovnik = (Suradnica*) malloc(57 * sizeof(Suradnica));
    initializeSuradnica(slovnik, 0 ,0);
    initializeSuradnica(slovnik, 0, 6);
    initializeSuradnica(slovnik,1, 6);
    initializeSuradnica(slovnik,2, 6);
    initializeSuradnica(slovnik,3, 6);
    initializeSuradnica(slovnik,4, 6);
    initializeSuradnica(slovnik,4, 7);
    initializeSuradnica(slovnik,4, 8);
    initializeSuradnica(slovnik,4, 9);
    initializeSuradnica(slovnik,4, 10);
    initializeSuradnica(slovnik,5, 10);
    initializeSuradnica(slovnik,6, 10);
    initializeSuradnica(slovnik,6, 9);
    initializeSuradnica(slovnik,6, 8);
    initializeSuradnica(slovnik,6, 7);
    initializeSuradnica(slovnik,6, 6);
    initializeSuradnica(slovnik,7, 6);
    initializeSuradnica(slovnik,8, 6);
    initializeSuradnica(slovnik,9, 6);
    initializeSuradnica(slovnik,10, 6);
    initializeSuradnica(slovnik,10, 5);
    initializeSuradnica(slovnik,10, 4);
    initializeSuradnica(slovnik,9, 4);
    initializeSuradnica(slovnik,8, 4);
    initializeSuradnica(slovnik,7, 4);
    initializeSuradnica(slovnik,6, 4);
    initializeSuradnica(slovnik,6, 3);
    initializeSuradnica(slovnik,6, 2);
    initializeSuradnica(slovnik,6, 1);
    initializeSuradnica(slovnik,6, 0);
    initializeSuradnica(slovnik,5, 0);
    initializeSuradnica(slovnik,4, 0);
    initializeSuradnica(slovnik,4, 1);
    initializeSuradnica(slovnik,4, 2);
    initializeSuradnica(slovnik,4, 3);
    initializeSuradnica(slovnik,4, 4);
    initializeSuradnica(slovnik,3, 4);
    initializeSuradnica(slovnik,2, 4);
    initializeSuradnica(slovnik,1, 4);
    initializeSuradnica(slovnik,0, 4);
    initializeSuradnica(slovnik,0, 5);
    initializeSuradnica(slovnik,1, 5); // vyherne domceky - modra
    initializeSuradnica(slovnik,2, 5);
    initializeSuradnica(slovnik,3, 5);
    initializeSuradnica(slovnik,4, 5);
    initializeSuradnica(slovnik,5, 1); // červená
    initializeSuradnica(slovnik,5, 2);
    initializeSuradnica(slovnik,5, 3);
    initializeSuradnica(slovnik,5, 4);
    initializeSuradnica(slovnik,5, 9); // zelená
    initializeSuradnica(slovnik,5, 8);
    initializeSuradnica(slovnik,5, 7);
    initializeSuradnica(slovnik,5, 6);
    initializeSuradnica(slovnik,9, 5); // oranžová
    initializeSuradnica(slovnik,8, 5);
    initializeSuradnica(slovnik,7, 5);
    initializeSuradnica(slovnik,6, 5);
    return slovnik;
}

Suradnica* getZapornySlovnik(const HernaPlocha* hernaplocha) {
    Suradnica* slovnik = (Suradnica*) malloc(16 * sizeof(Suradnica));
    initializeSuradnica(slovnik,0,9);
    initializeSuradnica(slovnik,0,10);
    initializeSuradnica(slovnik,1,9);
    initializeSuradnica(slovnik,1,10);
    initializeSuradnica(slovnik,0,0);
    initializeSuradnica(slovnik,0,1);
    initializeSuradnica(slovnik,1,0);
    initializeSuradnica(slovnik,1,1);
    initializeSuradnica(slovnik,9,9);
    initializeSuradnica(slovnik,9,10);
    initializeSuradnica(slovnik,10,9);
    initializeSuradnica(slovnik,10,10);
    initializeSuradnica(slovnik,9,0);
    initializeSuradnica(slovnik,9,1);
    initializeSuradnica(slovnik,10,0);
    initializeSuradnica(slovnik,10,1);
    return slovnik;
}

char getZnakPodlaCislaHraca(const HernaPlocha* hernaplocha, int cisloHraca) {
    switch(cisloHraca) {
        case 0: return 'M';
        case 1: return 'C';
        case 2: return 'Z';
        case 3: return 'O';
    }
    return '?';
}

void initializeSuradnica(Suradnica* suradnica, int r, int s) {
    suradnica->r = r;
    suradnica->s = s;
}