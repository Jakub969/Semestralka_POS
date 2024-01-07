#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include "buffer.h"
#include "pos_sockets/char_buffer.h"
#include "pos_sockets/active_socket.h"
#include "pos_sockets/passive_socket.h"
#include "hrac/hrac.h"
#include "hernaPlocha/hernaPlocha.h"

#define MAX_STRINGS 15

typedef struct Figurka{
    int cisloHraca;
    int startovaciaPozicia;
    int zaciatokKoncovehoDomceka;
    int domcekovaPozicia;
    int aktualnaPozicia;
    int pocetPrejdenychPolicok;
}FIGURKA;

void figurka_init(FIGURKA* figurka, int cisloHraca, int startovaciaPozicia, int domcekovaPozicia, int zaciatokKoncocehoDomceka) {
    figurka->cisloHraca = cisloHraca;
    figurka->aktualnaPozicia = domcekovaPozicia;
    figurka->pocetPrejdenychPolicok = 0;
    figurka->startovaciaPozicia = startovaciaPozicia;
    figurka->domcekovaPozicia = domcekovaPozicia;
    figurka->zaciatokKoncovehoDomceka = zaciatokKoncocehoDomceka;
}

typedef struct HraciaPlocha {
    FIGURKA* figurky;
    int pocetFiguriek;
    int pocetHracov;
}HRACIA_PLOCHA;

void hracia_plocha_init(HRACIA_PLOCHA *hraciaPlocha, int pocetHracov) {
    hraciaPlocha->pocetHracov = pocetHracov;
    hraciaPlocha->pocetFiguriek = pocetHracov * 4;
    hraciaPlocha->figurky = (FIGURKA*) calloc(hraciaPlocha->pocetFiguriek, sizeof(FIGURKA));
    for (int i = 0; i < pocetHracov; ++i) {
        for (int j = 0; j < 4; ++j) {
            int indexFigurky = i*4+j;
            figurka_init(&hraciaPlocha->figurky[indexFigurky], i, i*10+1, -(indexFigurky), 41+indexFigurky);
        }
    }
}

void hracia_plocha_destroy(HRACIA_PLOCHA* hraciaPlocha) {
    /*for (int i = 0; i < hraciaPlocha->pocetHracov; ++i) {
        figurka_destroy(&hraciaPlocha->figurky[i]);
    }*/
    free(hraciaPlocha->figurky);
}

typedef struct thread_data {
    int pocetHracov;
    int pocetPripravenychHracov;
    int hracNaRade;
    short port;
    ACTIVE_SOCKET* my_socket;
    _Bool koniec;
    _Bool vyhral;
    HRACIA_PLOCHA* hraciaPlocha;
    Hrac* serveroviHrac;
} THREAD_DATA;

void serveroviHracHadzKockou(THREAD_DATA *pData);

void thread_data_init(struct thread_data* data, int pocetHracov, short port, ACTIVE_SOCKET* my_socket, HRACIA_PLOCHA* hraciaPlocha, Hrac* serveroviHrac) {
    data->pocetHracov = pocetHracov;
    data->hracNaRade = 1;
    data->port = port;
    data->my_socket = my_socket;
    data->hraciaPlocha = hraciaPlocha;
    data->koniec = false;
    data->vyhral = false;
    data->serveroviHrac = serveroviHrac;
    data->pocetPripravenychHracov = 1;
}

void thread_data_destroy(struct thread_data* data) {
    data->port = 0;
    data->my_socket = NULL;
}

char dajOznacenie(int i) {
    switch (i) {
        case 1:
            return 'M';
        case 2:
            return 'C';
        case 3:
            return 'Z';
        case 4:
            return 'O';
        default:
            return '?';
    }
}

void* process_client_data(void* thread_data) {
    struct thread_data* data = (struct thread_data*)thread_data;
    PASSIVE_SOCKET sock_passive;
    passive_socket_init(&sock_passive);
    passive_socket_start_listening(&sock_passive, data->port);
    /*for (int i = 0; i < data->pocetHracov; ++i) {
        passive_socket_wait_for_client(&sock_passive, data->my_socket);
        CHAR_BUFFER buffer;
        char_buffer_init(&buffer);
        char poradie[50];
        sprintf(poradie, "%d", i+1);
        char_buffer_append(&buffer, poradie, strlen(poradie));
        char_buffer_append(&buffer, ";", strlen(";"));
        char oznacenie[50];
        char temp = dajOznacenie(i);
        sprintf(oznacenie, "%c", temp);
        char_buffer_append(&buffer, oznacenie, strlen(oznacenie));
        char_buffer_append(&buffer, "\0", 1);
        active_socket_write_data(data->my_socket, &buffer);
        char_buffer_destroy(&buffer);
        printf("Pripojil sa hrac %d s oznacenim %s\n", i+1, oznacenie);
    }*/
    passive_socket_wait_for_client(&sock_passive, data->my_socket);
    CHAR_BUFFER buffer;
    char_buffer_init(&buffer);
    char poradie[50];
    sprintf(poradie, "%d", 2);
    char_buffer_append(&buffer, poradie, strlen(poradie));
    char_buffer_append(&buffer, ";", strlen(";"));
    char oznacenie[50];
    char temp = dajOznacenie(2);
    sprintf(oznacenie, "%c", temp);
    char_buffer_append(&buffer, oznacenie, strlen(oznacenie));
    char_buffer_append(&buffer, "\0", 1);
    active_socket_write_data(data->my_socket, &buffer);
    char_buffer_destroy(&buffer);
    printf("Pripojil sa hrac %d s oznacenim %s\n", 2, oznacenie);
    passive_socket_stop_listening(&sock_passive);
    passive_socket_destroy(&sock_passive);

    active_socket_start_reading(data->my_socket);

    return NULL;
}

char** dekodujSpravu(const char* buffer, const char delimiter, size_t* numStrings) {
    char** strings = (char**)malloc(MAX_STRINGS * sizeof(char*));
    if (strings == NULL) {
        fprintf(stderr, "Alokovanie pamate zlyhalo\n");
        exit(EXIT_FAILURE);
    }

    size_t count = 0;
    char* token = strtok((char*)buffer, &delimiter);
    while (token != NULL) {
        strings[count] = strdup(token);
        if (strings[count] == NULL) {
            fprintf(stderr, "Alokovanie pamate zlyhalo\n");
            exit(EXIT_FAILURE);
        }
        count++;

        if (count >= MAX_STRINGS) {
            fprintf(stderr, "Prekrocene maximalne mnozstvo stringov\n");
            break;
        }

        token = strtok(NULL, &delimiter);
    }

    *numStrings = count;
    return strings;
}

void uvolniPamatPolaStringov(char** strings, size_t numStrings) {
    for (size_t i = 0; i < numStrings; ++i) {
        free(strings[i]);
    }
    free(strings);
}

void posliAktualnyStav(THREAD_DATA *pData) {
    CHAR_BUFFER odpoved;
    char_buffer_init(&odpoved);
    if (pData->vyhral == true) {
        char vyhra[50];
        char_buffer_clear(&odpoved);
        sprintf(vyhra, "vyhralHrac;%d", pData->hracNaRade);
        char_buffer_append(&odpoved, vyhra, strlen(vyhra));
        char_buffer_append(&odpoved, "\0", 1);
        active_socket_write_data(pData->my_socket, &odpoved);
    }
    char temp[50];
    sprintf(temp, "hernaPlocha;%d;%d;", pData->pocetHracov, pData->hracNaRade);
    char_buffer_append(&odpoved, temp, strlen(temp));
    for (int i = 0; i < pData->hraciaPlocha->pocetFiguriek; ++i) {
        int pozicia = pData->hraciaPlocha->figurky[i].aktualnaPozicia;
        sprintf(temp, "%d;", pozicia);
        char_buffer_append(&odpoved, temp, strlen(temp));
    }
    char_buffer_append(&odpoved, "\0", 1);
    sleep(1);
    active_socket_write_data(pData->my_socket, &odpoved);
    size_t size;
    char** param = dekodujSpravu(odpoved.data,';', &size);
    HernaPlocha hernaPlocha;
    initializeHernaPlocha(&hernaPlocha, param);
    vypisSa(&hernaPlocha);
    uvolniPamatPolaStringov(param,size);
    char_buffer_destroy(&odpoved);
    if (getIdHraca(pData->serveroviHrac) == pData->hracNaRade) {
        serveroviHracHadzKockou(pData);
    }
}

void vykonajZmeny(const char *poradieHraca, const char *hodKockou, THREAD_DATA *data) {
    if (data->hracNaRade == data->pocetHracov) {
        data->hracNaRade = 0;
    }
    data->hracNaRade++;
    //int hracNaRade = atoi(poradieHraca);
    int hracNaRade = data->hracNaRade;
    int hod = atoi(hodKockou);
    if (hod == 6) {
        for (int i = (hracNaRade - 1) * 4; i < (hracNaRade * 4); ++i) {
            if (data->hraciaPlocha->figurky[i].aktualnaPozicia == data->hraciaPlocha->figurky[i].domcekovaPozicia) {
                data->hraciaPlocha->figurky[i].aktualnaPozicia = data->hraciaPlocha->figurky[i].startovaciaPozicia;
                data->hraciaPlocha->figurky[i].pocetPrejdenychPolicok = 1;
                break;
            } else if (data->hraciaPlocha->figurky[i].aktualnaPozicia != data->hraciaPlocha->figurky[i].domcekovaPozicia && data->hraciaPlocha->figurky[i].aktualnaPozicia != data->hraciaPlocha->figurky[i].zaciatokKoncovehoDomceka) {
                int novaPozicia = (data->hraciaPlocha->figurky[i].aktualnaPozicia + hod) % 40;
                if (novaPozicia == 0) {
                    novaPozicia = 1;
                }
                data->hraciaPlocha->figurky[i].aktualnaPozicia = novaPozicia;
                data->hraciaPlocha->figurky[i].pocetPrejdenychPolicok += hod;
                if ((data->hraciaPlocha->figurky[i].pocetPrejdenychPolicok > 39)) {
                    data->hraciaPlocha->figurky[i].aktualnaPozicia = data->hraciaPlocha->figurky[i].zaciatokKoncovehoDomceka;
                }
                break;
            }
        }
    } else {
        for (int i = (hracNaRade - 1) * 4; i < (hracNaRade * 4); ++i) {
            if (data->hraciaPlocha->figurky[i].aktualnaPozicia != data->hraciaPlocha->figurky[i].domcekovaPozicia && data->hraciaPlocha->figurky[i].aktualnaPozicia != data->hraciaPlocha->figurky[i].zaciatokKoncovehoDomceka) {
                int novaPozicia = (data->hraciaPlocha->figurky[i].aktualnaPozicia + hod) % 40;
                if (novaPozicia == 0) {
                    novaPozicia = 1;
                }
                data->hraciaPlocha->figurky[i].aktualnaPozicia = novaPozicia;
                data->hraciaPlocha->figurky[i].pocetPrejdenychPolicok += hod;
                if ((data->hraciaPlocha->figurky[i].pocetPrejdenychPolicok > 39)) {
                    data->hraciaPlocha->figurky[i].aktualnaPozicia = data->hraciaPlocha->figurky[i].zaciatokKoncovehoDomceka;
                }
                break;
            }
        }
    }
    int pocetVKoncovom = 0;
    for (int j = (hracNaRade - 1) * 4; j < (hracNaRade * 4); ++j) {
        if (data->hraciaPlocha->figurky[j].aktualnaPozicia == data->hraciaPlocha->figurky[j].zaciatokKoncovehoDomceka) {
            pocetVKoncovom++;
        }
    }
    if (pocetVKoncovom == 4) {
        data->vyhral = true;
    }
    posliAktualnyStav(data);
}

void serveroviHracHadzKockou(THREAD_DATA *pData) {
    printf("Stlac 'e' aby si hodil kockou.\n");
    char klavesa = 't';
    while (klavesa != 'e') {
        scanf("%c", &klavesa);
    }
    char hracNaRade[1];
    sprintf(hracNaRade, "%d", pData->hracNaRade);
    char hod[1];
    sprintf(hod, "%d", hodKockou());
    printf("Hodil si cislo: %s\n", hod);
    vykonajZmeny(hracNaRade, hod, pData);
}

void vykonajInstrukciu(CHAR_BUFFER *buffer, THREAD_DATA *data) {
    printf("%s\n", buffer->data);
    CHAR_BUFFER odpoved;
    char_buffer_init(&odpoved);
    size_t numStrings;
    char** decodedStrings = dekodujSpravu(buffer->data, ';', &numStrings);

    if ((strcmp(decodedStrings[0], "hracPripraveny") == 0)) {
        data->pocetPripravenychHracov++;
        char odpovedaj[50];
        if (data->pocetPripravenychHracov == data->pocetHracov) {
            sprintf(odpovedaj, "%s", "Hra sa zacne za ...;");
            printf("Hra sa zacne za ...\n");
            char_buffer_append(&odpoved, odpovedaj, 21);
            active_socket_write_data(data->my_socket, &odpoved);
            char_buffer_clear(&odpoved);
            sleep(1);
            char_buffer_append(&odpoved, "3;", 3);
            printf("3\n");
            active_socket_write_data(data->my_socket, &odpoved);
            char_buffer_clear(&odpoved);
            sleep(1);
            char_buffer_append(&odpoved, "2;", 3);
            printf("2\n");
            active_socket_write_data(data->my_socket, &odpoved);
            char_buffer_clear(&odpoved);
            sleep(1);
            char_buffer_append(&odpoved, "1;", 3);
            printf("1\n");
            active_socket_write_data(data->my_socket, &odpoved);
            posliAktualnyStav(data);
        }
    } else if ((strcmp(decodedStrings[0], "hracCislo") == 0)) {
        vykonajZmeny(decodedStrings[1], decodedStrings[2], data);
    }
    uvolniPamatPolaStringov(decodedStrings, numStrings);
    char_buffer_destroy(&odpoved);
}


void prijmaj(struct thread_data *pData) {
    CHAR_BUFFER buffer;
    char_buffer_init(&buffer);
    if (active_socket_try_get_read_data(pData->my_socket, &buffer)) {
        if (active_socket_is_end_message(pData->my_socket,&buffer)) {
            active_socket_stop_reading(pData->my_socket);
            pData->koniec = true;
            char_buffer_destroy(&buffer);
            return;
        }
        vykonajInstrukciu(&buffer, pData);
    }
    char_buffer_destroy(&buffer);
}


int main() {
    pthread_t th_receive;
    int pocetHracov = 2;
    short port = 15874;
    HRACIA_PLOCHA hraciaPlocha;
    struct thread_data data;
    struct active_socket my_socket;
    Hrac serveroviHrac;
    initializeHrac(&serveroviHrac, 1, 'M');
    hracia_plocha_init(&hraciaPlocha, pocetHracov);
    active_socket_init(&my_socket);
    thread_data_init(&data, pocetHracov, port ,&my_socket, &hraciaPlocha, &serveroviHrac);

    pthread_create(&th_receive, NULL, process_client_data, &data);

    while (!data.koniec) {
        prijmaj(&data);
    }
    pthread_join(th_receive, NULL);

    hracia_plocha_destroy(&hraciaPlocha);
    thread_data_destroy(&data);
    active_socket_destroy(&my_socket);

    return 0;
}