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
    initializeSuradnica(&slovnik[0], 0 ,9);
    initializeSuradnica(&slovnik[1], 0, 6);
    initializeSuradnica(&slovnik[2],1, 6);
    initializeSuradnica(&slovnik[3],2, 6);
    initializeSuradnica(&slovnik[4],3, 6);
    initializeSuradnica(&slovnik[5],4, 6);
    initializeSuradnica(&slovnik[6],4, 7);
    initializeSuradnica(&slovnik[7],4, 8);
    initializeSuradnica(&slovnik[8],4, 9);
    initializeSuradnica(&slovnik[9],4, 10);
    initializeSuradnica(&slovnik[10],5, 10);
    initializeSuradnica(&slovnik[11],6, 10);
    initializeSuradnica(&slovnik[12],6, 9);
    initializeSuradnica(&slovnik[13],6, 8);
    initializeSuradnica(&slovnik[14],6, 7);
    initializeSuradnica(&slovnik[15],6, 6);
    initializeSuradnica(&slovnik[16],7, 6);
    initializeSuradnica(&slovnik[17],8, 6);
    initializeSuradnica(&slovnik[18],9, 6);
    initializeSuradnica(&slovnik[19],10, 6);
    initializeSuradnica(&slovnik[20],10, 5);
    initializeSuradnica(&slovnik[21],10, 4);
    initializeSuradnica(&slovnik[22],9, 4);
    initializeSuradnica(&slovnik[23],8, 4);
    initializeSuradnica(&slovnik[24],7, 4);
    initializeSuradnica(&slovnik[25],6, 4);
    initializeSuradnica(&slovnik[26],6, 3);
    initializeSuradnica(&slovnik[27],6, 2);
    initializeSuradnica(&slovnik[28],6, 1);
    initializeSuradnica(&slovnik[29],6, 0);
    initializeSuradnica(&slovnik[30],5, 0);
    initializeSuradnica(&slovnik[31],4, 0);
    initializeSuradnica(&slovnik[32],4, 1);
    initializeSuradnica(&slovnik[33],4, 2);
    initializeSuradnica(&slovnik[34],4, 3);
    initializeSuradnica(&slovnik[35],4, 4);
    initializeSuradnica(&slovnik[36],3, 4);
    initializeSuradnica(&slovnik[37],2, 4);
    initializeSuradnica(&slovnik[38],1, 4);
    initializeSuradnica(&slovnik[39],0, 4);
    initializeSuradnica(&slovnik[40],0, 5);
    initializeSuradnica(&slovnik[41],1, 5); // vyherne domceky - modra
    initializeSuradnica(&slovnik[42],2, 5);
    initializeSuradnica(&slovnik[43],3, 5);
    initializeSuradnica(&slovnik[44],4, 5);
    initializeSuradnica(&slovnik[45],5, 1); // červená
    initializeSuradnica(&slovnik[46],5, 2);
    initializeSuradnica(&slovnik[47],5, 3);
    initializeSuradnica(&slovnik[48],5, 4);
    initializeSuradnica(&slovnik[49],5, 9); // zelená
    initializeSuradnica(&slovnik[50],5, 8);
    initializeSuradnica(&slovnik[51],5, 7);
    initializeSuradnica(&slovnik[52],5, 6);
    initializeSuradnica(&slovnik[53],9, 5); // oranžová
    initializeSuradnica(&slovnik[54],8, 5);
    initializeSuradnica(&slovnik[55],7, 5);
    initializeSuradnica(&slovnik[56],6, 5);
    return slovnik;
}

Suradnica* getZapornySlovnik(const HernaPlocha* hernaplocha) {
    Suradnica* slovnik = (Suradnica*) malloc(16 * sizeof(Suradnica));
    initializeSuradnica(&slovnik[0], 0, 9);
    initializeSuradnica(&slovnik[1],0,10);
    initializeSuradnica(&slovnik[2],1,9);
    initializeSuradnica(&slovnik[3],1,10);
    initializeSuradnica(&slovnik[4],0,0);
    initializeSuradnica(&slovnik[5],0,1);
    initializeSuradnica(&slovnik[6],1,0);
    initializeSuradnica(&slovnik[7],1,1);
    initializeSuradnica(&slovnik[8],9,9);
    initializeSuradnica(&slovnik[9],9,10);
    initializeSuradnica(&slovnik[10],10,9);
    initializeSuradnica(&slovnik[11],10,10);
    initializeSuradnica(&slovnik[12],9,0);
    initializeSuradnica(&slovnik[13],9,1);
    initializeSuradnica(&slovnik[14],10,0);
    initializeSuradnica(&slovnik[15],10,1);
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