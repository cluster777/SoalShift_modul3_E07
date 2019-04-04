#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
int kekenyangan,kebersihan,hp,boleh=1,shop=10,punyaku=10,cd=0;
pthread_mutex_t makan,mandi,batle,cool,boleh_mandi;
pthread_t tid[10];
void* hitung_lapar(){
	sleep(10);
	pthread_mutex_lock(&makan);
	kekenyangan-=5;
	pthread_mutex_unlock(&makan);
}
void* hitung_kotor(){
	sleep(10);
	pthread_mutex_lock(&mandi);
	kebersihan-=10;
	pthread_mutex_unlock(&mandi);
}
void* hitung_regen(){
	sleep(10);
	pthread_mutex_lock(&batle);
	hp+=5;
	if(hp>300)hp=300;
	pthread_mutex_unlock(&batle);
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
void* battle(){
	
}
void* belanja(){
	
}
int main(){
	pthread_mutex_init(&makan,NULL);
	pthread_mutex_init(&mandi,NULL);
	pthread_mutex_init(&batle,NULL);
	pthread_mutex_init(&boleh_mandi,NULL);
	pthread_mutex_init(&cool,NULL);
	char *nama;
	scanf("%s",&nama);
	hp=300;
	kekenyangan=200;
	kebersihan=100;
	pthread_create(&(tid[0]),NULL,&hitung_lapar,NULL);
	pthread_create(&(tid[1]),NULL,&hitung_kotor,NULL);
	pthread_create(&(tid[2]),NULL,&hitung_regen,NULL);
	while(hp>0&&kebersihan>0&&kekenyangan>0){
		printf("stanby mode\nhealth : %d\nhunger : %d\nhygiene : %d\nfood left : %d\nbath will be rady in :%ds\nchoices\n1.eat\n2.bath\n3.battle\n4.shop\n5.exit\n",hp,kekenyangan,kebersihan,punyaku,cd);
		system("clear");
		int input=0;
		//get input
		if(input==1)//makan
		{
			pthread_mutex_lock(&makan);
			kekenyangan+=15;
			if(kekenyangan>200)kekenyangan=200;//?
			pthread_mutex_unlock(&makan);
		}
		else if(input==2 && boleh==1)//mandi
		{
			pthread_mutex_lock(&mandi);
			pthread_mutex_lock(&boleh_mandi);
			kebersihan+=30;
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
				printf("battle mode\nmonster's health : %d\nenemy's health : %d\nchoices\n 1.attack 2.run",hp,en);
				input=0;
				//get input
				system("clear");
				if(input==2)break;
				if(input==1){
					en-=20;
					printf("enemy hit damage 20\n");
					sleep(1);
					system("clear");
					if(en<=0)break;
					hp-=20;
					printf("%s hit damage 20\n",nama);
					if(hp<=0)break;
					sleep(1);
					system("clear");
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
				printf("shop mode\nshop food stock :%d\nyour food stock : %d\nchoices\n1.buy\n2.back\n",shop,punyaku);
				input=0;
				//get input
				if(input==2)break;
				if(input==1 && shop>0){
					punyaku++;
					shop--;
				}
				system("clear");
			}
			pthread_mutex_unlock(&batle);
		}
		else if(input==5)break;
		
	}
}
