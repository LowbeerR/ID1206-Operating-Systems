#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

#define MAX_MSG_SIZE 1024
#define QUEUE_NAME "/mymq"

int main(){

    mqd_t queue = mq_open(QUEUE_NAME, O_RDONLY, 0600);
    char rcvmsg[MAX_MSG_SIZE*10];

    mq_receive(queue, rcvmsg, MAX_MSG_SIZE*10, NULL);
    int count = 0;
    for(int i=0; rcvmsg[i]; i++) {
        if(rcvmsg[i] != ' ') {
            count ++;
        }
    }

    printf("%d\n",count);
    mq_close(queue);
    mq_unlink(QUEUE_NAME);

}