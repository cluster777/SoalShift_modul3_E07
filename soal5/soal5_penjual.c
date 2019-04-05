#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMKEY 11037

int input;
pthread_t tid[2];

char getch()
{
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
    struct termios old= {0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
}

void* xinput()
{
    input = (int)getch() - '0';
    return NULL;
}
void* xwait()
{
    sleep(1);
    pthread_cancel(tid[0]);
    return NULL;
}
int main()
{
    key_t key = SHMKEY;
    int shmid = shmget(key, sizeof key, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Gagal membuat shared memory");
        exit(EXIT_FAILURE);
    }
    int *value = shmat(shmid, NULL, 0);
    if (value == (void*)-1)
    {
        perror("Gagal meng-attach ke shared memory");
        exit(EXIT_FAILURE);
    }
    *value = 10;
    while(1)
    {
        system("clear");
        //create package
        //make connection
        //send package(request)
        //get answer
        //close connection
        printf("shop\nfood stock :%d\nchoices\n1.Restock\n2.Exit\n", *value);

        //printf("working\n");
        //create package
        input = 0;
        pthread_create(&(tid[0]), NULL, &xinput, NULL);
        pthread_create(&(tid[1]), NULL, &xwait, NULL);
        pthread_join(tid[1], NULL);
        tcflush(0, TCIFLUSH);
        //printf("%d\n",input);
        if(input == 2)
            break;
        else if(input == 1)
        {
            //create package
            //make connection
            //send package(send)
            //close connection
            (*value)++;
        }
    }
}
