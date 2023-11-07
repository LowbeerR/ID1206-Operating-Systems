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
#define MAX_MSG_SIZE 1024
#define QUEUE_NAME "/Queue"

//alternative (doesnt work)

int main(){
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        // read file here
        char *msg = "hej123"
                    "gf"
                    "dawdawjdoaw";
        char *my_mq = "/balle";

        /*struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_msgsize = 5000;
        attr.mq_maxmsg = 100;
        attr.mq_curmsgs = 0;*/
        printf("%d%s\n", errno, " rad 29");
        mqd_t queue = mq_open(my_mq, O_CREAT, NULL);
        printf("%d%s\n", errno, " rad 31");
        FILE *file;
        file = fopen("test.txt", "r");
        char content[MAX_MSG_SIZE];
        char bigboy[MAX_MSG_SIZE*10];
        //char *strncat(char *dest, const char *src, size_t n)
        while(fgets(content, MAX_MSG_SIZE, file)) {
            content[strcspn(content, "\n")] = 0;
            //printf("%s", content);
            strcat(bigboy, content);
        }

        mq_send(queue, bigboy, strlen(bigboy) + 1, 0);
        printf("%d%s\n", errno, " rad 43");
       // printf("\n%s\n", bigboy);
        fclose(file);
        mq_close(queue);
    }
    if (pid > 0){
        wait(NULL);

        char *my_mq = "/que";
        mqd_t queue = mq_open(my_mq, O_RDONLY);

        char rcvmsg[MAX_MSG_SIZE*10];
        mq_receive(queue, rcvmsg, MAX_MSG_SIZE*10, NULL);
       // printf("%d\n", errno);
        int count = 0;
        printf("%s", rcvmsg);
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
