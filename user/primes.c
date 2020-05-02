#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child_f(int*);

// EOF in a pipe is sent only when *all* processes
// are done with writing at the pipe's write end.
// Thus, childs processes *must* close it too, otherwise
// the child process is waiting for itself.
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
        child_f(parent_fd);
    }
    else {
        close(parent_fd[0]); // unused read end
        for (int i = 2; i < 35; i++) {
            write(parent_fd[1], &i, sizeof(i));
        }
        close(parent_fd[1]);
    }

    exit(0);
}

void child_f(int* pipefd) {
    int p;
    int n;

    close(pipefd[1]); // unused write end
    if ((read(pipefd[0], &p, sizeof(p))) <= 0) {
        printf("no p value received\n");
        exit(0);
    }

    printf("p = %d\n", p);

    int parent_fd[2];
    int child_pid;
    pipe(parent_fd);

    while ((read(pipefd[0], &n, sizeof(n))) > 0) {
        if (n % p != 0) {
            write(parent_fd[1], &n, sizeof(n)); 
        }
    }
    close(pipefd[0]);
    if ((child_pid = fork()) == -1) {
        printf("fork error\n");
        exit(0);
    }
    if (child_pid == 0) {
        child_f(parent_fd);
    }
    else {
        close(parent_fd[1]);
        close(parent_fd[0]);
        exit(0);
    }
}
