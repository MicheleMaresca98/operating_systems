//2 tipologie di thread: cons, prod. 
//5 istanze che fanno codice prod, 1 cons
//compito 7.7.11


#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PRODUTTORI 5
#define NUM_CONSUMATORI 1

#define TOTAL NUM_PRODUTTORI+NUM_CONSUMATORI

#define DIM 5
#define NUM_PROD 4
#define NUM_CONS 10


//ogni produttore deve fare 4 inserimenti intervallati da tot secondi
void*Produttore(void*arg){
	//casting
	Stack*s;
	s=(Stack*) arg;
	int i;
	Elem e;
	i=0;
	while(i<NUM_PROD){

		e=(Elem) (rand()%11);//si potrebbe anche omettere questo casting
		StackPush(s,e);
		printf("[PRODUTTORE] valore prodoto %d\n", e);
		sleep(1);//generiamo elemento dopo sleep
		i++;
	}
	pthread_exit(NULL);
}

void*Consumatore(void*arg){
	//casting
	Stack*s;
	s=(Stack*) arg;
	int i;
	i=0;
	Elem e1, e2;
	while(i<NUM_CONS){
		printf("[CONSUMATORE] Consumo n* %d\n",i+1);
		e1= StackPop(s);
		e2= StackPop(s);
		printf("[CONSUMATORE] HO consmato %d e %d e la somma è %d\n", e1,e2,e1+e2);
		
		i++;
	}
	pthread_exit(NULL);
}

int main(){

	pthread_t threads[TOTAL];
	pthread_attr_t attr;
	//creiamo puntatore stack
	Stack* s;
	Elem e;
	int i;
	i=0;
	srand(time(NULL));
	
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	s=(Stack*) malloc(sizeof(Stack));
	StackInit(s, DIM);
	for(i=0;i<TOTAL; i++){
		if(i==0){
			pthread_create(&threads[i], &attr, Consumatore, (void*)s);		
		}else{
			pthread_create(&threads[i], &attr, Produttore, (void*)s);
		}
	
	}
	for(i=0;i<TOTAL;i++){
		pthread_join(threads[i],NULL);
	}
	
	
	StackRemove(s);
	

	
	return 0;
}
