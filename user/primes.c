#include "kernel/types.h"
#include "user/user.h"

void primes(int input[], int num){
    if (num == 1)
    {
        printf("prime %d\n", input[0]);
        return;
    }
    int p[2], i;
    int prime = input[0];
    int temp;
    printf("prime %d\n", prime);
    pipe(p);
    if (fork() == 0)
    {
        for (i = 1; i < num; i++)
        {
            write(p[1], &input[i], sizeof(input[i]));
        }
        exit();
    }
    close(p[1]);
    if (fork() == 0)
    {
        int count = 0;
        while (read(p[0], &temp, sizeof(temp)) != 0)
        {
            if (temp % prime != 0)
            {
                input[count] = temp;
                count++;
            }
        }
        primes(input, count);
        exit();
    }
    wait();
    wait();
}


int main()
{
    int input[34];
    for (int i = 0; i < 34; i++)
    {
        input[i] = i + 2;
    }
    primes(input, 34);
    exit();
}

