#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
    if (argc != 2)
    {
        fprintf(2,"error massage\n");
        exit();
    }
    int time = atoi(argv[1]);
    sleep(time);
    exit();
}