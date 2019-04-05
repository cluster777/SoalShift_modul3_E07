#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
pthread_t tid[10];
void* p11(){
	system("ps -aux|head -10>>/root/Documents/salin1.txt");
}
void* p21(){
	system("cp /root/Documents/salin1.txt /root/Documents/salin2.txt");
}

void*p13(){
	system("unzip /root/Documents/zip1 -d /root/Documents");
	system("rm /root/Documents/zip1.zip");
}
void*p23(){
	system("unzip /root/Documents/zip2 -d /root/Documents");
	system("rm /root/Documents/zip2.zip");
}
void*p12(){
	system("zip /root/Documents/zip1 /root/Documents/salin1.txt");
	system("rm /root/Documents/salin1.txt");
	sleep(15);
	pthread_create(&(tid[2]),NULL,&p13,NULL);
	pthread_join(tid[2],NULL);
}
void*p22(){
	system("zip /root/Documents/zip2 /root/Documents/salin2.txt");
	system("rm /root/Documents/salin2.txt");
	sleep(15);
	pthread_create(&(tid[3]),NULL,&p23,NULL);
	pthread_join(tid[3],NULL);
}
int main(){
	pthread_create(&(tid[0]),NULL,&p11,NULL);
	pthread_join(tid[0],NULL);
	pthread_create(&(tid[1]),NULL,&p21,NULL);
	pthread_join(tid[1],NULL);
	pthread_create(&(tid[0]),NULL,&p12,NULL);
	pthread_create(&(tid[1]),NULL,&p22,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
}
