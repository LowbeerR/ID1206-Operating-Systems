#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int buffer = 0;
pthread_mutex_t lock;

void increment(void) {
    pthread_mutex_lock(&lock);
    buffer++;
    pthread_mutex_unlock(&lock);
}

int readBuffer(void) {
    pthread_mutex_lock(&lock);
    int ret = buffer;
    pthread_mutex_unlock(&lock);
    return ret;
}

void *thread_func(void *param) {

    while (buffer < 15) {
        if (pthread_mutex_lock(&lock) == 0) {
            if (buffer < 15) {
                int *count_ptr = (int *) param;
                printf("TID: %lu, PID: %d, Buffer: %d \n", pthread_self(), getpid(), buffer++);
                (*count_ptr)++;
            }
            pthread_mutex_unlock(&lock);
        }
    }
    return 0;
}

int main() {
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    pthread_t t1, t2, t3;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, &thread_func, &count1);
    pthread_create(&t2, NULL, &thread_func, &count2);
    pthread_create(&t3, NULL, &thread_func, &count3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock);

    printf("TID %d worked on the buffer %d times \n", 1, count1);
    printf("TID %d worked on the buffer %d times \n", 2, count2);
    printf("TID %d worked on the buffer %d times \n", 3, count3);
    printf("Total buffer accesses: %d \n", count1 + count2 + count3);
    return 0;
}