#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/mman.h>

int MAX = 3;
struct buffer {
    int VAR;
    int readCount;
};


int main() {
    shm_unlink("/sharedmem");
    size_t size = sizeof(struct buffer);
    int shared = shm_open("/sharedmem", O_CREAT | O_RDWR, 0666);
    ftruncate(shared, size);
    struct buffer *buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared, 0);

    buffer->VAR = 0;
    buffer->readCount = 0;

    //makes sure the semaphore is not already in use for something else.
    sem_unlink("mutex");
    sem_unlink("w_mutex");
    //Creates semaphores for the writer and the readers.
    sem_t *mutex = sem_open("mutex", O_CREAT, 0600, 1);
    sem_t *w_mutex = sem_open("w_mutex", O_CREAT, 0600, 1);
    pid_t r1, r2;
    r1 = fork();
    if (r1 > 0) {
        r2 = fork();
        if (r2 > 0) {
            //writer
            while (true) {
                sem_wait(w_mutex);
                if (buffer->VAR + 1 == MAX) {
                    kill(r1, 0);
                    kill(r2, 0);
                    return 0;
                } else {
                    printf("The writer acquires the lock\n");
                    printf("The writer (%d) writes the value %d\n", getpid(), ++buffer->VAR);
                    printf("The writer releases the lock\n");
                    sem_post(w_mutex);
                    sleep(3);
                }
            }
        } else if (r2 == 0) {
            //Reader2
            while (true) {
                sem_wait(mutex);
                if (++buffer->readCount == 1) {
                    sem_wait(w_mutex);
                    printf("The first reader acquires the lock.\n");
                }
                sem_post(mutex);
                printf("The reader (%d) reads the value %d\n", getpid(), buffer->VAR);
                sem_wait(mutex);
                if (--buffer->readCount == 0) {
                    sem_post(w_mutex);
                    printf("The last reader releases the lock.\n");
                }
                sem_post(mutex);
                sleep(2);
            }
        }
    } else if (r1 == 0) {
        while (true) {
            sem_wait(mutex);
            buffer->readCount++;
            if (buffer->readCount == 1) {
                sem_wait(w_mutex);
                printf("The first reader acquires the lock.\n");
            }
            sem_post(mutex);
            printf("The reader (%d) reads the value %d\n", getpid(), buffer->VAR);
            sem_wait(mutex);
            buffer->readCount--;
            if (buffer->readCount == 0) {
                sem_post(w_mutex);
                printf("The last reader releases the lock.\n");
            }
            sem_post(mutex);
            sleep(2);
        }
    }
    return 0;
}