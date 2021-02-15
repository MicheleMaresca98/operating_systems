#ifndef __HEADER
#define __HEADER

#include <pthread.h>
typedef int Elem;

typedef struct Stack{
	Elem* dati;
	int dim;
	pthread_mutex_t mutex;
	pthread_cond_t ok_prod;
	pthread_cond_t ok_cons;
	int testa;
}Stack;

void StackInit(Stack *s, int dim);
void StackRemove(Stack *s);
void StackPush(Stack *s, Elem e);
Elem StackPop(Stack *s);
int StackSize(Stack *s);

#endif
