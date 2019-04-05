#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
#include<sys/ioctl.h>
int kekenyangan,kebersihan,hp,boleh=1,shop=10,punyaku=10,cd=0,input=0;
pthread_mutex_t makan,mandi,batle,cool,boleh_mandi,toko;
pthread_t tid[10];
void* hitung_lapar(){
	while(1){
	sleep(10);
	pthread_mutex_lock(&makan);
	kekenyangan-=5;
	pthread_mutex_unlock(&makan);
}
}
void* hitung_kotor(){
	while(1){
	sleep(30);
	pthread_mutex_lock(&mandi);
	kebersihan-=10;
	pthread_mutex_unlock(&mandi);
}
}
void* hitung_regen(){
	while(1){
	sleep(10);
	pthread_mutex_lock(&batle);
	hp+=5;
	if(hp>300)hp=300;
	pthread_mutex_unlock(&batle);
}
}
void* gaboleh(){
	while(cd>0){
		sleep(1);
		pthread_mutex_lock(&cool);
		cd--;
		pthread_mutex_unlock(&cool);
	}
	pthread_mutex_lock(&boleh_mandi);
	boleh=1;
	pthread_mutex_unlock(&boleh_mandi);
}
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
 void*clerk(){
	 //make port and ready to connect
	 //while(1){
	 //wait for connection
	 //ifgetconnection{
	 //make connection
	 
	 /*if(receive==0){
	  * make status package
	  * send
	  * }
	  * else if(receive==1){
	  * pthread_mutex_lock(&toko);
	  * shop++;
	  * pthread_mutex_unlock(&toko);
	  * }
	 */
	 
	 //get close connection
	 //}
	 //}
 }
void*xinput(){
	input=(int)getch()-'0';
}
void*xwait(){
	sleep(1);
	pthread_cancel(tid[4]);
}
int main(){
	
	//menu_win=newwin(30,30,20,20);
	pthread_mutex_init(&makan,NULL);
	pthread_mutex_init(&mandi,NULL);
	pthread_mutex_init(&batle,NULL);
	pthread_mutex_init(&boleh_mandi,NULL);
	pthread_mutex_init(&cool,NULL);
	pthread_mutex_init(&toko,NULL);
	char *nama;
	printf("inputkan nama>");
	scanf("%s",&nama);

	hp=300;
	kekenyangan=200;
	kebersihan=100;
	pthread_create(&(tid[0]),NULL,&hitung_lapar,NULL);
	pthread_create(&(tid[1]),NULL,&hitung_kotor,NULL);
	pthread_create(&(tid[2]),NULL,&hitung_regen,NULL);
	pthread_create(&(tid[6]),NULL,&clerk,NULL);
	//printf("asd");
	while(hp>0&&kebersihan>0&&kekenyangan>0){
		system("clear");

		printf("standby mode\nhealth : %d\nhunger : %d\nhygiene : %d\nfood left : %d\nbath will be ready in :%ds\nchoices\n1.eat\n2.bath\n3.battle\n4.shop\n5.exit\n",hp,kekenyangan,kebersihan,punyaku,cd);

		input=0;
		pthread_create(&(tid[4]),NULL,&xinput,NULL);
		pthread_create(&(tid[5]),NULL,&xwait,NULL);
		pthread_join(tid[5],NULL);
		tcflush(0,TCIFLUSH);
		if(input==1&&punyaku>0)//makan
		{
			pthread_mutex_lock(&makan);
			kekenyangan+=15;
			punyaku--;
			if(kekenyangan>200)kekenyangan=200;//?
			pthread_mutex_unlock(&makan);
		}
		else if(input==2 && boleh==1)//mandi
		{
			pthread_mutex_lock(&mandi);
			pthread_mutex_lock(&boleh_mandi);
			kebersihan+=30;
			if(kebersihan>100)kebersihan=100;
			boleh=0;
			cd=20;
			pthread_create(&(tid[3]),NULL,&gaboleh,NULL);
			pthread_mutex_unlock(&mandi);
		}
		else if(input==3){//battle
			pthread_mutex_lock(&batle);
			pthread_mutex_lock(&mandi);
			pthread_mutex_lock(&makan);
			int en=100;
			while(1){
				system("clear");
				printf("battle mode\nmonster's health : %d\nenemy's health : %d\nchoices\n 1.attack\n 2.run\n",hp,en);
				input=0;
				//get input
				input=0;
				pthread_create(&(tid[4]),NULL,&xinput,NULL);
				pthread_create(&(tid[5]),NULL,&xwait,NULL);
				pthread_join(tid[5],NULL);
				tcflush(0,TCIFLUSH);
				
				if(input==2)break;
				if(input==1){
					en-=20;
					//printf("enemy hit damage 20\n");
					sleep(1);
					//system("clear");
					if(en<=0)break;
					hp-=20;
					//printf("%s hit damage 20\n",nama);
					if(hp<=0)break;
					//sleep(1);
					//system("clear");
				}
				
			}
			if(hp<=0)break;
			pthread_mutex_unlock(&batle);
			pthread_mutex_unlock(&mandi);
			pthread_mutex_unlock(&makan);
		}
		else if(input==4)//shop
		{
			//pembeli jelas banget
			pthread_mutex_lock(&batle);
			while(1){
				system("clear");
				printf("shop mode\nshop food stock :%d\nyour food stock : %d\nchoices\n1.buy\n2.back\n",shop,punyaku);
				input=0;
				//get input
				input=0;
				pthread_create(&(tid[4]),NULL,&xinput,NULL);
				pthread_create(&(tid[5]),NULL,&xwait,NULL);
				pthread_join(tid[5],NULL);
				tcflush(0,TCIFLUSH);
				if(input==2)break;
				if(input==1 && shop>0){
					punyaku++;
					shop--;
				}
				
			}
			pthread_mutex_unlock(&batle);
		}
		else if(input-'0'==5)break;
		
	}
}
