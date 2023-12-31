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
    if (sscanf(buf->data, "%d;%d", pi_estimation->total_count, pi_estimation->inside_count) == 2) {
        return true;
    }
    return false;
}


GENERATE_BUFFER(struct point, point)


typedef struct thread_data {
    long long replications_count;
    struct buffer_point buf;
    pthread_mutex_t mutex;
    pthread_cond_t is_full;
    pthread_cond_t is_empty;

    short port;
    ACTIVE_SOCKET* my_socket;
} THREAD_DATA;

void thread_data_init(struct thread_data* data, long long replications_count, int buffer_capacity, short port, ACTIVE_SOCKET* my_socket) {
    data->replications_count = replications_count;
    buffer_point_init(&data->buf, buffer_capacity);
    pthread_mutex_init(&data->mutex, NULL);
    pthread_cond_init(&data->is_full, NULL);
    pthread_cond_init(&data->is_empty, NULL);

    data->port = port;
    data->my_socket = my_socket;
}

void thread_data_destroy(struct thread_data* data) {
    data->replications_count = 0;
    buffer_point_destroy(&data->buf);
    pthread_mutex_destroy(&data->mutex);
    pthread_cond_destroy(&data->is_full);
    pthread_cond_destroy(&data->is_empty);

    data->port = 0;
    data->my_socket = NULL;
}

void* process_client_data(void* thread_data) {
    struct thread_data* data = (struct thread_data*)thread_data;
    PASSIVE_SOCKET sock_passive;
    passive_socket_init(&sock_passive);
    passive_socket_start_listening(&sock_passive, data->port);
    //TODO pre kazdeho hraca sa musí zavolat listening
    passive_socket_wait_for_client(&sock_passive, data->my_socket);
    passive_socket_stop_listening(&sock_passive);
    passive_socket_destroy(&sock_passive);

    active_socket_start_reading(data->my_socket);

    return NULL;
}

void* produce(void* thread_data) {
    struct thread_data* data = (struct thread_data*)thread_data;

    for (long long i = 1; i <= data->replications_count; ++i) {
        POINT item = generate_point();

        pthread_mutex_lock(&data->mutex);
        while (!buffer_point_try_push(&data->buf, item)) {
            pthread_cond_wait(&data->is_empty, &data->mutex);
        }
        pthread_cond_signal(&data->is_full);
        pthread_mutex_unlock(&data->mutex);
    }
    return NULL;
}

_Bool try_get_client_pi_estimation(struct active_socket* my_sock, struct pi_estimation* client_pi_estimaton) {

    CHAR_BUFFER buf;
    char_buffer_init(&buf);
    _Bool result = false;
    if (active_socket_try_get_read_data(my_sock, &buf)) {
        if (!pi_estimation_try_deserialize(client_pi_estimaton, &buf)) {
            if (active_socket_is_end_message(my_sock, &buf)) {
                active_socket_stop_reading(my_sock);
            }
        } else {
            result = true;
        }
    }


    char_buffer_destroy(&buf);


    return result;
}

void* consume(void* thread_data) {
    struct thread_data* data = (struct thread_data*)thread_data;

    struct pi_estimation pi_estimaton = {0, 0};
    struct pi_estimation client_pi_estimaton = {0, 0};
    for (long long i = 1; i <= data->replications_count; ++i) {
        POINT item;

        pthread_mutex_lock(&data->mutex);
        while (!buffer_point_try_pop(&data->buf, &item)) {
            pthread_cond_wait(&data->is_full, &data->mutex);
        }
        pthread_cond_signal(&data->is_empty);
        pthread_mutex_unlock(&data->mutex);

        printf("%ld: ", i);
        ++pi_estimaton.total_count;
        if (item.x * item.x + item.y * item.y <= 1) {
            ++pi_estimaton.inside_count;
        }
        printf("Odhad pi: %lf\n", 4 * (double)pi_estimaton.inside_count / (double)pi_estimaton.total_count);

        if (data->my_socket != NULL) {
            try_get_client_pi_estimation(data->my_socket, &client_pi_estimaton);
            printf("%ld: ", i);
            printf("Odhad pi s vyuzitim dat od klienta: %lf\n",
                   4 * (double)(pi_estimaton.inside_count + client_pi_estimaton.inside_count) /
                   (double)(pi_estimaton.total_count + client_pi_estimaton.total_count));
        }
    }
    if (data->my_socket != NULL) {
        while (active_socket_is_reading(data->my_socket)) {
            if (try_get_client_pi_estimation(data->my_socket, &client_pi_estimaton)) {
                printf("Odhad pi s vyuzitim dat od klienta: %lf\n",
                       4 * (double)(pi_estimaton.inside_count + client_pi_estimaton.inside_count) /
                       (double)(pi_estimaton.total_count + client_pi_estimaton.total_count));
            }
        }
    }

    return NULL;
}


int main(int argc, char* argv[]) {
    //printf("Hello");
    pthread_t th_produce;
    pthread_t th_receive;
    struct thread_data data;
    struct active_socket my_socket;

    active_socket_init(&my_socket);
    thread_data_init(&data, 100000, 10, 12345, &my_socket);

    pthread_create(&th_produce, NULL, produce, &data);
    pthread_create(&th_receive, NULL, process_client_data, &data);

    consume(&data);

    pthread_join(th_produce, NULL);
    pthread_join(th_receive, NULL);

    thread_data_destroy(&data);
    active_socket_destroy(&my_socket);

    return 0;
}
