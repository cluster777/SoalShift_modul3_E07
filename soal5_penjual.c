#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
#include<sys/ioctl.h>
int input,stock;
char getch(){
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
    struct termios old={0};
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
void*xinput(){
	input=(int)getch()-'0';
}
void*xwait(){
	sleep(1);
	pthread_cancel(tid[4]);
}
int main(){
	
	while(1){
		system("clear");
		//create package
		//make connection
		//send package(request)
		//get answer
		//close connection
		printf("working\n");
		//create package
		input=0;
		pthread_create(&(tid[4]),NULL,&xinput,NULL);
		pthread_create(&(tid[5]),NULL,&xwait,NULL);
		pthread_join(tid[5],NULL);
		tcflush(0,TCIFLUSH);
		//printf("%d\n",input);
		if(input==2)break;
		else if(input==1){
			//create package
			//make connection
			//send package(send)
			//close connection
		}
	}
}
}
