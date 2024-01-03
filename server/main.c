#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "buffer.h"
#include "pos_sockets/char_buffer.h"
#include "pos_sockets/active_socket.h"
#include "pos_sockets/passive_socket.h"

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
    int cisloHraca;
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

typedef struct thread_data {
    int pocetHracov;
    int pocetPripravenychHracov;
    short port;
    ACTIVE_SOCKET* my_socket;
    _Bool koniec;
    HRACIA_PLOCHA* hraciaPlocha;
} THREAD_DATA;

void thread_data_init(struct thread_data* data, int pocetHracov, short port, ACTIVE_SOCKET* my_socket, HRACIA_PLOCHA* hraciaPlocha) {
    data->pocetHracov = pocetHracov;
    data->pocetPripravenychHracov = 0;
    data->port = port;
    data->my_socket = my_socket;
    data->hraciaPlocha = hraciaPlocha;
    data->koniec = false;
}

void thread_data_destroy(struct thread_data* data) {
    data->port = 0;
    data->my_socket = NULL;
}

void* process_client_data(void* thread_data) {
    struct thread_data* data = (struct thread_data*)thread_data;
    PASSIVE_SOCKET sock_passive;
    passive_socket_init(&sock_passive);
    passive_socket_start_listening(&sock_passive, data->port);
    for (int i = 0; i < data->pocetHracov; ++i) {
        passive_socket_wait_for_client(&sock_passive, data->my_socket);
        CHAR_BUFFER buffer;
        char_buffer_init(&buffer);
        char poradie[50];
        sprintf(poradie, "%d", i+1);
        char_buffer_append(&buffer, poradie, strlen(poradie));
        char_buffer_append(&buffer, ";", strlen(";"));
        char oznacenie[50];
        sprintf(oznacenie, "%c", data->hraciaPlocha->zaciatocnyDomcek[i].figurka[0].figurka);
        char_buffer_append(&buffer, oznacenie, strlen(oznacenie));
        char_buffer_append(&buffer, "\0", 1);
        active_socket_write_data(data->my_socket, &buffer);
        printf("Pripojil sa hrac %d s oznacenim %s\n", i+1, oznacenie);
    }
    passive_socket_stop_listening(&sock_passive);
    passive_socket_destroy(&sock_passive);

    active_socket_start_reading(data->my_socket);

    return NULL;
}


void posliPolicka(THREAD_DATA *pData) {
    CHAR_BUFFER odpoved;
    char_buffer_init(&odpoved);
    char policka[50];
    sprintf(policka, "%s", "hernaPlocha;");
    char_buffer_append(&odpoved, policka, strlen(policka));
    sprintf(policka, "%d", pData->pocetHracov);
    char_buffer_append(&odpoved, policka, strlen(policka));
    char_buffer_append(&odpoved, ";", strlen(";"));
    for (int i = 0; i < pData->pocetHracov; ++i) {
        sprintf(policka, "%d", pData->hraciaPlocha->zaciatocnyDomcek[i].pocetFiguriekVDomceku);
        char_buffer_append(&odpoved, policka, strlen(policka));
        char_buffer_append(&odpoved, ";", strlen(";"));
    }
    for (int i = 0; i < pData->pocetHracov; ++i) {
        sprintf(policka, "%d", pData->hraciaPlocha->koncovyDomcek[i].pocetFiguriekVDomceku);
        char_buffer_append(&odpoved, policka, strlen(policka));
        char_buffer_append(&odpoved, ";", strlen(";"));
    }
    for (int i = 0; i < 52; ++i) {
        sprintf(policka, "%c", pData->hraciaPlocha->policka[i]);
        char_buffer_append(&odpoved, policka, strlen(policka));
        char_buffer_append(&odpoved, ";", strlen(";"));
    }
    active_socket_write_data(pData->my_socket, &odpoved);
}

void vykonajInstrukciu(CHAR_BUFFER *buffer, THREAD_DATA *data) {
    printf("%s\n", buffer->data);
    CHAR_BUFFER odpoved;
    char_buffer_init(&odpoved);
    if (strcmp(buffer->data, "Hrac 1 je pripraveny") == 0 || strcmp(buffer->data, "Hrac 2 je pripraveny") == 0 || strcmp(buffer->data, "Hrac 3 je pripraveny") == 0 || strcmp(buffer->data, "Hrac 4 je pripraveny") == 0) {
        data->pocetPripravenychHracov++;
        char odpovedaj[50];
        if (data->pocetPripravenychHracov == data->pocetHracov) {
            sprintf(odpovedaj, "%s", "Hra sa zacne za ...\n");
            char_buffer_append(&odpoved,odpovedaj, 21);
            active_socket_write_data(data->my_socket, &odpoved);
            char_buffer_clear(&odpoved);
            sleep(1);
            char_buffer_append(&odpoved, "3\n", 3);
            active_socket_write_data(data->my_socket, &odpoved);
            char_buffer_clear(&odpoved);
            sleep(1);
            char_buffer_append(&odpoved, "2\n", 3);
            active_socket_write_data(data->my_socket, &odpoved);
            char_buffer_clear(&odpoved);
            sleep(1);
            char_buffer_append(&odpoved, "1\n", 3);
            active_socket_write_data(data->my_socket, &odpoved);
            posliPolicka(data);
        } else  {
            sprintf(odpovedaj, "%s", "Je pripravenych ");
            sprintf(odpovedaj, "%d", data->pocetPripravenychHracov);
            sprintf(odpovedaj, "%c", '/');
            sprintf(odpovedaj, "%d", data->pocetHracov);
            sprintf(odpovedaj, "%s", " hracov\n");
            char_buffer_append(&odpoved,odpovedaj, strlen(odpovedaj));
            active_socket_write_data(data->my_socket, &odpoved);
        }
    }

}


void prijmaj(struct thread_data *pData) {
    CHAR_BUFFER buffer;
    char_buffer_init(&buffer);
    if (active_socket_try_get_read_data(pData->my_socket, &buffer)) {
        if (active_socket_is_end_message(pData->my_socket,&buffer)) {
            active_socket_stop_reading(pData->my_socket);
            pData->koniec = true;
            return;
        }
        vykonajInstrukciu(&buffer, pData);
    }
    char_buffer_destroy(&buffer);
}


int main() {
    pthread_t th_receive;
    int pocetHracov = 1;
    short port = 15874;
    HRACIA_PLOCHA hraciaPlocha;
    struct thread_data data;
    struct active_socket my_socket;

    init_hracia_plocha(&hraciaPlocha, pocetHracov);
    active_socket_init(&my_socket);
    thread_data_init(&data, pocetHracov, port ,&my_socket, &hraciaPlocha);

    pthread_create(&th_receive, NULL, process_client_data, &data);
    while (!data.koniec) {
        prijmaj(&data);
    }
    pthread_join(th_receive, NULL);

    thread_data_destroy(&data);
    active_socket_destroy(&my_socket);

    return 0;
}
