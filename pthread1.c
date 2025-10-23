// pthread1.c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* worker(void* arg) {
    const char* name = (const char*)arg;
    for (int i = 0; i < 5; i++) {
        printf("[%s] i=%d\n", name, i);
        usleep(100000); // 0.1s so you can see interleaving
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // create two threads
    pthread_create(&t1, NULL, worker, "T1");
    pthread_create(&t2, NULL, worker, "T2");

    // wait for them to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("[main] done\n");
    return 0;
}
