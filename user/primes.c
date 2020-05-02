#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child_f(int);

int main(int argc, char *argv[])
{
    int parent_fd[2];
    int child_pid;

    pipe(parent_fd);

    if ((child_pid = fork()) == -1) {
        printf("fork error\n");
        exit(0);
    }

    if (child_pid == 0) {
        child_f(parent_fd[0]);
    }
    else {
        // feed first 35 numbers to first child process
        for (int i = 2; i < 35; i++) {
            write(parent_fd[1], &i, sizeof(i));
        }
        int test = close(parent_fd[1]);
    }

    exit(0);
}

void child_f(int fd) {
    int p;
    int n;

    if ((read(fd, &p, sizeof(p))) <= 0) {
        printf("no p value received\n");
        exit(0);
    }

    printf("p = %d\n", p);

    int parent_fd[2];
    int child_pid;
    pipe(parent_fd);
    while ((read(fd, &n, sizeof(n))) > 0) {
        if (n % p != 0) {
            int bytes = write(parent_fd[1], &n, sizeof(n)); 
            printf("number of bytes written: %d\n", bytes);
        }
        printf("test\n");
    }
    if ((child_pid = fork()) == -1) {
        printf("fork error\n");
        exit(0);
    }
    if (child_pid == 0) {
        printf("forking\n");
        child_f(parent_fd[0]);
    }
    else {
        close(parent_fd[1]);
        exit(0);
    }
}

// int main(int argc, char *argv[])
// {
//     int child_pid;
//     int parent_fd[2];
//     int child_fd[2];
//     char byte = 'a';
//     char buffer[10];

//     // fd[0] is for reading (ugh!)
//     pipe(parent_fd);
//     pipe(child_fd);

//     if ((child_pid = fork()) == -1) {
//         printf("fork error\n");
//         exit(0);
//     }

//     // child process
//     if (child_pid == 0) {
//         int nbytes = read(parent_fd[0], &buffer, sizeof(buffer));
//         if (nbytes > 0) {
//             printf("%d: received ping\n", getpid(), buffer[0]);

//             // send byte back
//             write(child_fd[1], &byte, sizeof(byte));
//         }
//     }
//     // parent process
//     else {
//         write(parent_fd[1], &byte, sizeof(byte));

//         // read back the byte sent
//         int nbytes = read(child_fd[0], &buffer, sizeof(buffer));
//         if (nbytes > 0) {
//             printf("%d: received pong\n", getpid(), buffer[0]);
//         }
//     }
//     exit(0);
// }
