#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>


int main(){
   // FILE *ptr;
    //FILE *file;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        // read file here
        struct mq_attr attr;

        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = 50;
        attr.mq_curmsgs = 0;
        char *msg = "hello";
        char *my_mq = "/my_mq";

        mqd_t queue = mq_open(my_mq, O_CREAT | O_WRONLY, &attr);
        mq_send(queue, msg, strlen(msg) + 1, 0);
        mq_close(queue);
    }
    if (pid > 0){
        wait(NULL);

        char *my_mq = "/my_mq";
        mqd_t queue = mq_open(my_mq, O_RDONLY);

        char rcvmsg[50];

        mq_receive(queue, rcvmsg, 50, NULL);
        int count = 0;
        for(int i=0; rcvmsg[i]; i++) {
        if(rcvmsg[i] != ' ') {
           count ++;
        }
        }


        printf("%s\n%d\n", rcvmsg,count);
        mq_close(queue);


    }

    return 0;
}
