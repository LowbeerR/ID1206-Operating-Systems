#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#define MAX_MSG_SIZE 1024
#define QUEUE_NAME "/mymq"
#define FILE_PATH "test.txt"

int main(){
    mqd_t queue = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0600, NULL);
    FILE *file;
    file = fopen(FILE_PATH, "r");
    char content[MAX_MSG_SIZE];
    char bigboy[MAX_MSG_SIZE*10];
    while(fgets(content, MAX_MSG_SIZE, file)) {
        content[strcspn(content, "\n")] = 0;
        strcat(bigboy, content);
    }
    mq_send(queue, bigboy, strlen(bigboy) + 1, 0);
    fclose(file);

    mq_close(queue);
}