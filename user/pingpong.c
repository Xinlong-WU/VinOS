#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int p1[2];
    int p2[2];

    pipe(p1);
    pipe(p2);
    if(fork() == 0){// ckild
        char * str = "";
        close(p1[1]);
        close(p2[0]);
        read(p1[0],str,1);
        printf("pid: %d, receivxed pong\n", getpid());
        write(p2[1],str, 1);

        close(p1[0]);
        close(p2[0]);
    }
    else{
        char * str = "";
        close(p1[0]);
        close(p2[1]);
        write(p1[1], " ", 1);

        read(p2[0],str,1);
        printf("pid: %d, received ping\n", getpid());

        close(p1[1]);
        close(p2[0]);

        wait(0);
    }
    exit(0);
}