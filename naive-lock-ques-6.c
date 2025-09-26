#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

int lock = 0;       // 0: unlocked, 1: locked
int shared = 0;     // biến dùng chung

void * incrementer(void * args){
    int i;
    for(i=0;i<100;i++){
        // đoạn găng (critical section)

        // spin-wait cho tới khi lock mở
        while(lock > 0);   // bận chờ

        lock = 1;          // giữ lock
        shared++;          // cập nhật biến chung
        lock = 0;          // trả lock
    }
    return NULL;
}

int main(int argc, char * argv[]){
    pthread_t * threads;
    int n,i;

    if(argc < 2){
        fprintf(stderr, "ERROR: Invalid number of threads\n");
        exit(1);
    }

    // chuyển argv[1] sang số nguyên
    if((n = atol(argv[1])) == 0){
        fprintf(stderr, "ERROR: Invalid number of threads\n");
        exit(1);
    }

    // cấp phát mảng chứa danh tính luồng
    threads = calloc(n,sizeof(pthread_t));

    // tạo n luồng
    for(i=0;i<n;i++){
        pthread_create(&threads[i], NULL, incrementer, NULL);
    }

    // chờ tất cả luồng kết thúc
    for(i=0;i<n;i++){
        pthread_join(threads[i], NULL);
    }

    // in kết quả
    printf("Shared: %d\n", shared);
    printf("Expect: %d\n", n*100);

    free(threads);
    return 0;
}
