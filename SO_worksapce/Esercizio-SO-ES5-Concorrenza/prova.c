#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

int main() {
    for(int i=0; i<5; i++){
        int pid = fork();
        if(pid==0){
            printf("getpid: %d \n", getpid());
            exit(1);
        }
    }

	return 0;
}
