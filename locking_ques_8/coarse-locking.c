#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 4
#define NUM_TRANS   10000000   // số giao dịch mỗi thread

long long shared_balance = 0;          // biến chung
pthread_mutex_t lock;                  // chỉ 1 lock cho toàn bộ

void *worker(void *arg) {
    long i;
    for (i = 0; i < NUM_TRANS; i++) {
        pthread_mutex_lock(&lock);
        shared_balance++;              // vùng găng
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

double get_time_in_ms() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000.0 + t.tv_usec / 1000.0;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    double start = get_time_in_ms();

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    double end = get_time_in_ms();

    printf("Coarse Locking: Final balance = %lld\n", shared_balance);
    printf("Execution time: %.2f ms\n", end - start);

    pthread_mutex_destroy(&lock);
    return 0;
}
