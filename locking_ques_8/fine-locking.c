#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 4
#define NUM_TRANS   10000000
#define NUM_BUCKETS 8            // số "ô" của tài khoản

long long buckets[NUM_BUCKETS];           // mỗi bucket có lock riêng
pthread_mutex_t bucket_locks[NUM_BUCKETS];

void *worker(void *arg) {
    long i;
    for (i = 0; i < NUM_TRANS; i++) {
        int idx = rand() % NUM_BUCKETS;    // chọn bucket ngẫu nhiên
        pthread_mutex_lock(&bucket_locks[idx]);
        buckets[idx]++;                    // chỉ lock bucket cần thiết
        pthread_mutex_unlock(&bucket_locks[idx]);
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

    for (int i = 0; i < NUM_BUCKETS; i++) {
        buckets[i] = 0;
        pthread_mutex_init(&bucket_locks[i], NULL);
    }

    double start = get_time_in_ms();

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    double end = get_time_in_ms();

    long long total = 0;
    for (int i = 0; i < NUM_BUCKETS; i++)
        total += buckets[i];

    printf("Fine Locking: Final balance = %lld\n", total);
    printf("Execution time: %.2f ms\n", end - start);

    for (int i = 0; i < NUM_BUCKETS; i++)
        pthread_mutex_destroy(&bucket_locks[i]);

    return 0;
}
