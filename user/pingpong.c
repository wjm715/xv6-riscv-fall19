#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int p1[2], p2[2];
    char buf[4];
    pipe(p1);
    pipe(p2);
    if (fork() == 0)
    {
        close(p1[1]);
        close(p2[0]);
        read(p1[0], buf, 5);
        printf("%d: received %s\n", getpid(), buf);
        write(p2[1], "pong", 5);
        exit();
    }
    close(p1[0]);
    close(p2[1]);
    write(p1[1], "ping", 5);
    read(p2[0], buf, 5);
    printf("%d: received %s\n", getpid(), buf);
    exit();
}
