#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void nPipe(int argc, char *argv[]);

char whitespace[] = " \t\r\n\v";

int getcmd(char *buf, int nbuf)
{
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if(buf[0] == 0) // EOF
        return -1;
    return 0;
}

void redirect(int k, int pd[])
{
    close(k);
    dup(pd[k]);
    close(pd[0]);
    close(pd[1]);
}

void preprocess(int *argc, char *argv[], char *buf)
{
    int i, j;
    for (i = 0, j = 0; buf[j] != '\n' && buf[j] != '\0'; j++)
    {
        while (strchr(whitespace, buf[j]))
        {
            j++;
        }
        argv[i] = buf + j;
        i++;
        while (strchr(whitespace, buf[j]) == 0)
        {
            j++;
        }
        buf[j] = '\0';
    }
    argv[i] = 0;
    *argc = i;
}

void runcmd(int argc, char* argv[])
{
    int i;
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            nPipe(argc, argv);
        }
    }
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], ">") == 0)
        {
            close(1);
            open(argv[i+1], O_CREATE|O_WRONLY);
            argv[i] = 0;
        }
        if (strcmp(argv[i], "<") == 0)
        {
            close(0);
            open(argv[i+1], O_RDONLY);
            argv[i] = 0;
        }
    }
    exec(argv[0], argv);
}

void nPipe(int argc, char *argv[])
{
    int i;
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            argv[i] = 0;
            break;
        }
    }
    int pd[2];
    pipe(pd);
    if (fork() == 0)
    {
        redirect(1,pd);
        runcmd(i, argv);
    }
    else
    {
        redirect(0,pd);
        runcmd(argc-i-1, argv+i+1);
    }
}

int main()
{
    char buf[100];
    while(getcmd(buf, sizeof(buf)) >= 0)
    {
        if(fork() == 0)
        {
            char *argv[10];
            int argc = -1;
            preprocess(&argc, argv, buf);
            runcmd(argc, argv);
        }
        wait(0);
    }
    exit(0);
}

