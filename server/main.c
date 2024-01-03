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

typedef struct point {
    double x;
    double y;
} POINT;

POINT generate_point(void) {
    double x = 2 * (rand() / (double)RAND_MAX) - 1;
    double y = 2 * (rand() / (double)RAND_MAX) - 1;
    POINT point = {x, y};
    return point;
}


typedef struct pi_estimation {
    long long total_count;
    long long inside_count;
} PI_ESTIMATION_DATA;

void pi_estimation_add_point(POINT data, struct pi_estimation* output_data) {
    ++output_data->total_count;
    if (data.x * data.x + data.y * data.y <= 1) {
        ++output_data->inside_count;
    }
    printf("Odhad pi: %f\n", 4 * (double)output_data->inside_count / (double)output_data->total_count);
}

_Bool pi_estimation_try_deserialize(struct pi_estimation* pi_estimation, struct char_buffer* buf) {
    if (sscanf(buf->data, "%lld;%lld", &pi_estimation->total_count, &pi_estimation->inside_count) == 2) {
        return true;
    }
    return false;
}


GENERATE_BUFFER(struct point, point)


typedef struct thread_data {
    int pocetHracov;
    short port;
    ACTIVE_SOCKET* my_socket;
    _Bool koniec;
} THREAD_DATA;

void thread_data_init(struct thread_data* data, int pocetHracov, short port, ACTIVE_SOCKET* my_socket) {
    data->pocetHracov = pocetHracov;
    data->port = port;
    data->my_socket = my_socket;
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
        char_buffer_append(&buffer, "\0", 1);
        active_socket_write_data(data->my_socket, &buffer);
        printf("Pripojil sa hrac %d\n", i+1);
    }
    passive_socket_stop_listening(&sock_passive);
    passive_socket_destroy(&sock_passive);

    active_socket_start_reading(data->my_socket);

    return NULL;
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

        printf("%s\n", buffer.data);
    }
    char_buffer_destroy(&buffer);
}

int main() {
    pthread_t th_receive;
    int pocetHracov = 1;
    short port = 15874;
    struct thread_data data;
    struct active_socket my_socket;

    active_socket_init(&my_socket);
    thread_data_init(&data, pocetHracov, port ,&my_socket);

    pthread_create(&th_receive, NULL, process_client_data, &data);
    while (!data.koniec) {
        prijmaj(&data);
    }
    pthread_join(th_receive, NULL);

    thread_data_destroy(&data);
    active_socket_destroy(&my_socket);

    return 0;
}
