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

void * transactions(void * args) {
    int i, v;
    for (i = 0; i < NUM_TRANS; i++) {
        // chọn giá trị ngẫu nhiên
        srand(time(NULL));
        v = rand() % NUM_TRANS;

        // ngẫu nhiên chọn cộng hoặc trừ
        if (rand() % 2) {
            // credit
            balance = balance + v;
            credits = credits + v;
        } else {
            // debit
            balance = balance - v;
            debits = debits + v;
        }
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

    free(threads);
    return 0;
}
