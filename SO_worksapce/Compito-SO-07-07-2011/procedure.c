#include "header.h"
#include <stdlib.h>
#include <stdio.h>

void StackInit(Stack*s, int dim){//inizializzare mutex, var condition e siccome abbiamo un puntatore dobbiamo istanziarlo e alla fine deallochiamo tutto
	
	pthread_mutex_init(&(s->mutex), NULL);
	pthread_cond_init(&(s->ok_prod), NULL);
	pthread_cond_init(&(s->ok_cons), NULL);
	
	s->testa=0;
	s->dim=dim;
	s->dati=(Elem*) malloc(dim*sizeof(Elem));//istanziamo puntatore
}

void StackRemove(Stack *s){
	pthread_mutex_destroy(&(s->mutex));
	pthread_cond_destroy(&(s->ok_prod));
	pthread_cond_destroy(&(s->ok_cons));
	free(s->dati);
}

void StackPush(Stack *s, Elem e){//singolo inserimento stack
	pthread_mutex_lock(&(s->mutex));//lock sul mutex
	while(s->testa == s->dim){//quando il prod si deve mettere in attesa?quando è piena(testa=coda)
		pthread_cond_wait(&(s->ok_prod),&(s->mutex));
	}
	s->dati[s->testa]=e;
	s->testa=s->testa+1;
	pthread_cond_signal(&(s->ok_cons));	//dobbiamo segnalare dopo aver prodotto
	pthread_mutex_unlock(&(s->mutex));
}

Elem StackPop(Stack *s){
	Elem ret;
	pthread_mutex_lock(&(s->mutex));
	
	while(s->testa == 0){
		pthread_cond_wait(&(s->ok_cons), &(s->mutex));
	}
	s->testa--;
	ret= s->dati[s->testa];
	pthread_cond_signal(&(s->ok_prod));
	pthread_mutex_unlock(&(s->mutex));
	return ret;
}

int StackSize(Stack *s){
	//bisogna accedere con mutex
	int d;
	pthread_mutex_lock(&(s->mutex));
	d=s->testa;
	pthread_mutex_unlock(&(s->mutex));
	return d;
}










