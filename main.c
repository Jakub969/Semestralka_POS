#include <stdio.h>
#include <stdlib.h>

typedef struct Figurka{
    char figurka;
    int cisloHraca;
    int startovaciaPozicia;
}FIGURKA;

typedef struct ZaciatocnyDomcek {
    int pocetFiguriekVDomceku;
    FIGURKA figurka[4];
}ZACIATOCNY_DOMCEK;

typedef struct KoncovyDomcek {
    int pocetFiguriekVDomceku;
}KONCOVY_DOMCEK;

typedef struct HraciaPlocha {
    char policka[52];
    int pocetHracov;
    ZACIATOCNY_DOMCEK zaciatocnyDomcek[4];
    KONCOVY_DOMCEK koncovyDomcek[4];
}HRACIA_PLOCHA;

void init_hracia_plocha(HRACIA_PLOCHA *hraciaPlocha, int pocetHracov) {
    hraciaPlocha->pocetHracov = pocetHracov;
    for (int i = 0; i < pocetHracov; i++) {
        char oznacenieFigurky;
        switch (i) {
            case 0:
                oznacenieFigurky = 'M'; //Označenie pre modrú figurku
                break;
            case 1:
                oznacenieFigurky = 'C'; //Označenie pre červenú figurku
                break;
            case 2:
                oznacenieFigurky = 'Z';  //Označenie pre zelenú figurku
                break;
            case 3:
                oznacenieFigurky = 'O'; //Označenie pre oranžovú figurku
                break;
            default:
                oznacenieFigurky = '*';
                break;
        }
        for (int j = 0; j < 4; j++) {
            hraciaPlocha->zaciatocnyDomcek[i].figurka[j].figurka = oznacenieFigurky;
            hraciaPlocha->zaciatocnyDomcek[i].figurka[j].cisloHraca = i + 1;
            hraciaPlocha->zaciatocnyDomcek[i].figurka[j].startovaciaPozicia = i * 13;
        }
        hraciaPlocha->zaciatocnyDomcek[i].pocetFiguriekVDomceku = 4;
        hraciaPlocha->koncovyDomcek[i].pocetFiguriekVDomceku = 0;
    }
    for (int i = 0; i < 52; i++) {
        hraciaPlocha->policka[i] = '*';
    }
}

void vypisHraciuPlochu(HRACIA_PLOCHA* hraciaPlocha) {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (((i < 6 || i > 8) && (j == 6 || j == 8)) || ((j < 6 || j > 8) && (i == 6 || i == 8))) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}


int main(int argc, char* argv[]) {

    int pocetHracov = atoi(argv[1]);

    printf("Pocet hracov: %d\n", pocetHracov);

    HRACIA_PLOCHA hraciaPlocha;

    init_hracia_plocha(&hraciaPlocha, pocetHracov);
    vypisHraciuPlochu(&hraciaPlocha);
}
