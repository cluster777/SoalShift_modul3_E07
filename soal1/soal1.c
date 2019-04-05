#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

pthread_t tid[100];

struct isi{
	int hitung;
	int pos;
};

void* fact(void* arg){
	int hasil=1,base=((struct isi*)arg)->hitung;
	
	while(base>1){
		hasil*=base;
		base--;
	} 
	if(((struct isi*)arg)->pos>0)pthread_join(tid[((struct isi*)arg)->pos-1],NULL);
	printf("%d %d! = %d\n",((struct isi*)arg)->pos,((struct isi*)arg)->hitung,hasil);
}

int main(){

	int i=0,arr[100];
	while((scanf("%d",&arr[i]))==1)i++;
	//srot bubble sip gak pake susah
	
	for(int j=0;j<i;j++)
		for(int k=j+1;k<i;k++){
			if(arr[j]>arr[k]){
				int temp=arr[j];
				arr[j]=arr[k];
				arr[k]=temp;
			}
		}
	//printf("%d",i);
	for(int j=0;j<i;j++){
		struct isi *test=(struct isi*)malloc(sizeof(struct isi));
		test->hitung=arr[j];
		test->pos=j;
		//printf("%d %d\n",test->start,test->stop);
		if(!pthread_create(&(tid[j]),NULL,&fact,(void*)test))
		{
			//printf("err\n");
		}
		else{
			//printf("success\n");
		}
		//sip
	}
	pthread_join(tid[i-1],NULL);
	printf("kelar\n");
}
