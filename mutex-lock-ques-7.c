#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define INIT_BALANCE 50
#define NUM_TRANS 10000

int balance = INIT_BALANCE;
int credits = 0;
int debits = 0;

pthread_mutex_t lock;   // <--- khai báo mutex

void *transactions(void * args) {
    int i, v;
    for (i = 0; i < NUM_TRANS; i++) {
        // chọn giá trị ngẫu nhiên
        v = rand() % NUM_TRANS;     // tránh srand nhiều lần, đặt srand ở main

        // ngẫu nhiên chọn cộng hoặc trừ
        pthread_mutex_lock(&lock);   // <--- khóa trước khi cập nhật biến chung

        if (rand() % 2) {
            // credit
            balance = balance + v;
            credits = credits + v;
        } else {
            // debit
            balance = balance - v;
            debits = debits + v;
        }

        pthread_mutex_unlock(&lock); // <--- mở khóa sau khi xong
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int n_threads, i;
    pthread_t *threads;

    if (argc < 2) {
        fprintf(stderr, "ERROR: Require number of threads\n");
        exit(1);
    }

    n_threads = atol(argv[1]);
    if (n_threads <= 0) {
        fprintf(stderr, "ERROR: Invalid value for number of threads\n");
        exit(1);
    }

    threads = calloc(n_threads, sizeof(pthread_t));

    srand(time(NULL));               // khởi tạo seed random ở main
    pthread_mutex_init(&lock, NULL); // <--- khởi tạo mutex

    // start all threads
    for (i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, transactions, NULL);
    }

    // wait for all threads
    for (i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\tCredits:\t%d\n", credits);
    printf("\tDebits:\t\t%d\n\n", debits);
    printf("%d + %d - %d =\t%d\n", INIT_BALANCE, credits, debits,
           INIT_BALANCE + credits - debits);
    printf("\tBalance:\t%d\n", balance);

    pthread_mutex_destroy(&lock);    // <--- huỷ mutex
    free(threads);
    return 0;
}
