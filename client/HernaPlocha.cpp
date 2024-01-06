//
// Created by Daniel on 6. 1. 2024.
//

#include "HernaPlocha.h"

HernaPlocha::HernaPlocha(std::vector<std::string> &parametre) {
    this->alokujPole();
    this->pocetHracov = std::stoi(parametre[1]);
    this->hracNaRade = std::stoi(parametre[2]);
    this->aktualizujSa(parametre);
}

void HernaPlocha::aktualizujSa(std::vector<std::string> &parametre) {

    this->nastavDefaultnePolicka();

    this->pocetHracov = std::stoi(parametre[1]);
    this->hracNaRade = std::stoi(parametre[2]);
    for (int hracI = 0; hracI < this->pocetHracov; hracI++) {
        for (int i = 0; i < 4; i++) {
            int indexPolicka = std::stoi(parametre[3 + (hracI * 4) + i]);
            Suradnica sur = this->getSuradnicuPodlaIndexu(indexPolicka);
            this->pole[sur.r][sur.s] = this->getZnakPodlaCislaHraca(hracI);
        }
    }

}


void HernaPlocha::nastavDefaultnePolicka() {
    // hviezdičky:
    char znakPolicka = '*';
    char znakVyhernehoDomceku = '.';
    char znakDomceka = '+';

    for (int i = 0; i < 11; ++i) {
        this->pole[4][i] = znakPolicka;
        this->pole[6][i] = znakPolicka;

        this->pole[i][4] = znakPolicka;
        this->pole[i][6] = znakPolicka;
    }

    this->pole[5][0] = znakPolicka;
    this->pole[5][10] = znakPolicka;
    this->pole[0][5] = znakPolicka;
    this->pole[10][5] = znakPolicka;

    // výherné domčeky:
    for(int i = 0; i < 4; i++) {
        this->pole[5][i + 1] = znakVyhernehoDomceku;
        this->pole[5][i + 6] = znakVyhernehoDomceku;
        this->pole[i + 1][5] = znakVyhernehoDomceku;
        this->pole[i + 6][5] = znakVyhernehoDomceku;
    }

    // domčeky:
    this->pole[0][0] = znakDomceka;
    this->pole[0][1] = znakDomceka;
    this->pole[1][0] = znakDomceka;
    this->pole[1][1] = znakDomceka;

    this->pole[0][9] = znakDomceka;
    this->pole[0][10] = znakDomceka;
    this->pole[1][9] = znakDomceka;
    this->pole[1][10] = znakDomceka;

    this->pole[9][0] = znakDomceka;
    this->pole[9][1] = znakDomceka;
    this->pole[10][0] = znakDomceka;
    this->pole[10][1] = znakDomceka;

    this->pole[9][9] = znakDomceka;
    this->pole[9][10] = znakDomceka;
    this->pole[10][9] = znakDomceka;
    this->pole[10][10] = znakDomceka;
}

HernaPlocha::~HernaPlocha() {
    for (int r = 0; r < this->velkostMapy; r++) {
        for (int s = 0; s < this->velkostMapy; s++) {
            this->pole[r][s] = 0;
        }
        free(this->pole[r]);
    }
    free(this->pole);
}



void HernaPlocha::vypisSa() {
    std::cout << "Pocet hracov: " << this->pocetHracov << std::endl;
    std::cout << "Na tahu je hrac cislo " << this->hracNaRade << " s farbou " << this->getFarbuHracaNaRade() << std::endl;
    for (int r = 0; r < this->velkostMapy; ++r) {
        for (int s = 0; s < this->velkostMapy; ++s) {
            std::cout << this->pole[r][s] << " ";
        }
        std::cout << std::endl;
    }
}

void HernaPlocha::alokujPole() {
    this->pole = static_cast<char **>(calloc(this->velkostMapy, sizeof(char *)));
    for (int r = 0; r < this->velkostMapy; r++) {
        this->pole[r] = (char*) calloc(this->velkostMapy, sizeof(char));
        for (int s = 0; s < this->velkostMapy; s++) {
            this->pole[r][s]= ' ';
        }
    }
}

std::string HernaPlocha::getFarbuHracaNaRade() {
    switch(this->hracNaRade) {
        case 1: return "Modry";
        case 2: return "Cerveny";
        case 3: return "Zeleny";
        case 4: return "Oranzovy";
    }
    return "CHYBA neplatne cislo hraca na rade!!!!";
}

Suradnica HernaPlocha::getSuradnicuPodlaIndexu(int index) {
    std::vector<Suradnica> slovnik = this->getSlovnik();
    std::vector<Suradnica> slovnikZaporny = this->getZapornySlovnik();

    if (index > 0) {
        return slovnik[index];
    } else {
        return slovnikZaporny[abs(index)];
    }
}

std::vector<Suradnica> HernaPlocha::getSlovnik() {
    std::vector<Suradnica> slovnik = {
            Suradnica(),
            Suradnica(0, 6),
            Suradnica(1, 6),
            Suradnica(2, 6),
            Suradnica(3, 6),
            Suradnica(4, 6),
            Suradnica(4, 7),
            Suradnica(4, 8),
            Suradnica(4, 9),
            Suradnica(4, 10),
            Suradnica(5, 10),
            Suradnica(6, 10),
            Suradnica(6, 9),
            Suradnica(6, 8),
            Suradnica(6, 7),
            Suradnica(6, 6),
            Suradnica(7, 6),
            Suradnica(8, 6),
            Suradnica(9, 6),
            Suradnica(10, 6),
            Suradnica(10, 5),
            Suradnica(10, 4),
            Suradnica(9, 4),
            Suradnica(8, 4),
            Suradnica(7, 4),
            Suradnica(6, 4),
            Suradnica(6, 3),
            Suradnica(6, 2),
            Suradnica(6, 1),
            Suradnica(6, 0),
            Suradnica(5, 0),
            Suradnica(4, 0),
            Suradnica(4, 1),
            Suradnica(4, 2),
            Suradnica(4, 3),
            Suradnica(4, 4),
            Suradnica(3, 4),
            Suradnica(2, 4),
            Suradnica(1, 4),
            Suradnica(0, 4),
            Suradnica(0, 5),
            Suradnica(1, 5), // vyherne domceky - modra
            Suradnica(2, 5),
            Suradnica(3, 5),
            Suradnica(4, 5),
            Suradnica(5, 1), // červená
            Suradnica(5, 2),
            Suradnica(5, 3),
            Suradnica(5, 4),
            Suradnica(5, 9), // zelená
            Suradnica(5, 8),
            Suradnica(5, 7),
            Suradnica(5, 6),
            Suradnica(9, 5), // oranžová
            Suradnica(8, 5),
            Suradnica(7, 5),
            Suradnica(6, 5),
    };
    return slovnik;
}

std::vector<Suradnica> HernaPlocha::getZapornySlovnik() {
    std::vector<Suradnica> slovnik = {
            Suradnica(0, 9), // modrý
            Suradnica(0, 10),
            Suradnica(1, 9),
            Suradnica(1, 10),
            Suradnica(0, 0), // červený
            Suradnica(0, 1),
            Suradnica(1, 0),
            Suradnica(1, 1),
            Suradnica(9, 9), // zelený
            Suradnica(9, 10),
            Suradnica(10, 9),
            Suradnica(10, 10),
            Suradnica(9, 0), // oranžový
            Suradnica(9, 1),
            Suradnica(10, 0),
            Suradnica(10, 1)
    };
    return slovnik;
}

char HernaPlocha::getZnakPodlaCislaHraca(int cisloHraca) {
    switch(cisloHraca) {
        case 0: return 'M';
        case 1: return 'C';
        case 2: return 'Z';
        case 3: return 'O';
    }
    return '?';
}


Suradnica::Suradnica(int r, int s) {
    this->r = r;
    this->s = s;
}

