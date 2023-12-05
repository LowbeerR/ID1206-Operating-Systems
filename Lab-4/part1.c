#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_CYL 5000;
#define MIN_CYL 0;
int QUEUE_LEN = 1000;

int *createQueue(int queue[]) {
    for (int i = 0; i < 1000; i++) {
        int r = rand() % 5001;
        queue[i] = r;
    }
    return queue;
}

//RÄTT
int FCFS(int header, int que[]) {
    int movement = 0;
    for (int i = 0; i < sizeof(que); i++) {
        movement += abs(que[i] - header);
        header = que[i];
    }

    return movement;
}

int SSTF(int header, int que[]) {
    int movement = 0;
    int destroy = 0;
    int check[sizeof(que)] = {0};

    for (int i = 0; i < sizeof(que); i++) {
        int minDist = INT_MAX;
        for (int j = 0; j < sizeof(que); j++) {
            if (check[j] != 1) {
                int temp = abs(que[j] - header);
                if (temp < minDist) {
                    minDist = temp;
                    destroy = j;

                }
            }
        }
        header = que[destroy];
        check[destroy] = 1;
        //que[destroy] = INT_MAX;
        movement += minDist;
    }
    return movement;
}

int SCAN(int header, int que[]) {
    int movement = 0;
    int current = header;
    int counter = 0;
    int check[sizeof(que)] = {0};
    while (counter != sizeof(que)) {
        if (current-- == 0)
            break;
        for (int j = 0; j < sizeof(que); j++) {
            if (que[j] == current && check[j] != 1) {
                movement += abs(que[j] - header);
                header = que[j];
                check[j] = 1;
                //que[j] = INT_MAX;
                counter++;
                break;
            }
        }
    }
    movement += header;
    header = 0;
    current = 0;
    while (counter != sizeof(que)) {
        if (current++ == 5000)
            break;
        for (int j = 0; j < sizeof(que); j++) {
            if (que[j] == current && check[j] != 1) {
                movement += abs(que[j] - header);
                header = que[j];
                check[j] = 1;
                //que[j] = INT_MAX;
                counter++;
                break;
            }
        }
    }
    return movement;
}

int CSCAN(int header, int que[]) {
    int movement = 0;
    int current = header;
    int counter = 0;
    int check[sizeof(que)] = {0};
    while (counter != sizeof(que)) {
        current++;
        if (current == 5000) {
            movement += abs(header - 5000);
            movement += 5000;
            header = 0;
            current = 0;
        }
        for (int j = 0; j < sizeof(que); j++) {
            if (que[j] == current && check[j] != 1) {
                movement += abs(que[j] - header);
                header = que[j];
                check[j] = 1;
                //que[j] = INT_MAX;
                counter++;
                break;
            }
        }
    }
    return movement;
}

int Look(int header, int que[]) {
    int movement = 0;
    int current = header;
    int counter = 0;
    int check[sizeof(que)] = {0};
    while (counter != sizeof(que)) {
        current++;
        if (current == 5000)
            break;
        for (int j = 0; j < sizeof(que); j++) {
            if (que[j] == current && check[j] != 1) {
                movement += abs(que[j] - header);
                header = que[j];
                check[j] = 1;
                //que[j] = INT_MAX;
                counter++;
                break;
            }
        }
    }
    while (counter != sizeof(que)) {
        if (current-- == 0)
            break;
        for (int j = 0; j < sizeof(que); j++) {
            if (que[j] == current && check[j] != 1) {
                movement += abs(que[j] - header);
                header = que[j];
                check[j] = 1;
                //que[j] = INT_MAX;
                counter++;
                break;
            }
        }
    }
    return movement;
}

int CLook(int header, int que[]) {
    int movement = 0;
    int current = header;
    int counter = 0;
    int check[sizeof(que)] = {0};
    while (counter != sizeof(que)) {
        if (current++ == 5000) {
            current = 0;
        }

        for (int j = 0; j < sizeof(que); j++) {
            if (que[j] == current && check[j] != 1) {
                movement += abs(que[j] - header);
                header = que[j];
                check[j] = 1;
                //que[j] = INT_MAX;
                counter++;
                break;
            }
        }
    }
    return movement;
}

int main(int argc, char *argv[]) {
    int queue[1000];
    int testQueue0[8] = {75, 90, 40, 135, 50, 170, 65, 10};
    createQueue(queue);
    if (argc >= 2) {
        int head = atoi(argv[1]);
        printf("FCFS, movement required: %d\n", FCFS(head, queue));
        printf("SSTF, movement required: %d\n", SSTF(head, queue));
        printf("CSCAN, movement required: %d\n", CSCAN(head, queue));
        printf("SCAN, movement required: %d\n", SCAN(head, queue));
        printf("CLook, movement required: %d\n", CLook(head, queue));
        printf("Look, movement required: %d\n", Look(head, queue));
    }

    return 0;
}
