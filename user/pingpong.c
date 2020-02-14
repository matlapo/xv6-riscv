#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int child_pid;
    int parent_fd[2];
    int child_fd[2];
    char byte = 'a';
    char buffer[10];

    // fd[0] is for reading (ugh!)
    pipe(parent_fd);
    pipe(child_fd);

    if ((child_pid = fork()) == -1) {
        printf("fork error\n");
        exit(0);
    }

    // child process
    if (child_pid == 0) {
        int nbytes = read(parent_fd[0], &buffer, sizeof(buffer));
        if (nbytes > 0) {
            printf("%d: received ping\n", getpid(), buffer[0]);

            // send byte back
            write(child_fd[1], &byte, sizeof(byte));
        }
    }
    // parent process
    else {
        write(parent_fd[1], &byte, sizeof(byte));

        // read back the byte sent
        int nbytes = read(child_fd[0], &buffer, sizeof(buffer));
        if (nbytes > 0) {
            printf("%d: received pong\n", getpid(), buffer[0]);
        }
    }
    exit(0);
}
