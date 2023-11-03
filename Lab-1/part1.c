#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>


int main() {
    pid_t pid;
    int pip[2];
    pipe(pip);
    pid = fork();
    if (pid == 0) {
        close(pip[0]);
        dup2(pip[1], STDOUT_FILENO);
        execl("/bin/ls", "ls", "/", NULL);
        return 0;
    }
    if (pid > 0) {
        wait(NULL);
        close(pip[1]);
        dup2(pip[0], STDIN_FILENO);
        execl("/usr/bin/wc", "wc", "-l", NULL);
        return 0;
    }
}
