#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


int main(){
    struct mq_attr {
        long mq_flags; // Message queue description flags
        long mq_maxmsg; // Maximum number of messages on queue
        long mq_msgsize; // Maximum message size (in bytes)
        long mq_curmsgs; // Number of messages currently in queue
    };

    pid_t pid;
    mqd_t mq_open();
    pid = fork();
    if (pid == 0) {
        //first process = child process
        //read context of file and pass it
    }
    if (pid > 0){
        wait(NULL);
    }


    return 0;
}
