#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


// wait
void Wait_Sem(int semid, int semnum) {
	struct sembuf s;

	s.sem_flg = 0;
	s.sem_num = semnum;	// SPAZIO_DISPONIBILE
	s.sem_op = -1;

	int r = semop(semid, &s, 1);
	
	if(r<0) { perror("ERRORE signal"); }
}

// wait for zero
void Wait_For_Zero_Sem(int semid, int semnum) {
	struct sembuf s;

	s.sem_flg = 0;
	s.sem_num = semnum;	// SPAZIO_DISPONIBILE
	s.sem_op = 0;   // wait for zero

	int r = semop(semid, &s, 1);
	
	if(r<0) { perror("ERRORE signal"); }
}


// signal
void Signal_Sem(int semid, int semnum) {
	struct sembuf s;

	s.sem_flg = 0;
	s.sem_num = semnum;
	s.sem_op = +1;

	int r = semop(semid, &s, 1);

	if(r<0) { perror("ERRORE signal"); }
}

void Signal_Sem_Neg(int semid, int semnum) {
	struct sembuf s;

	s.sem_flg = 0;
	s.sem_num = semnum;
	s.sem_op = -1;

	int r = semop(semid, &s, 1);

	if(r<0) { perror("ERRORE signal"); }
}



// produttore
void produttore(struct prodcons * p, int ds_sem, int column_index) {
	int indice = 0;
	Wait_Sem(ds_sem, SPAZIO_DISPONIBILE);
	Wait_Sem(ds_sem, MUTEX_P);
	while(indice <= DIM_COLONNE && p->stato[indice] != BUFFER_VUOTO) {
		indice++;
	}
	p->stato[indice] = BUFFER_INUSO;
	Signal_Sem(ds_sem, MUTEX_P);
	int sum = 0;
    for(int i=0; i<DIM_RIGHE; i++){
        sum += p->matrix[i][column_index];
    }
	printf("Proccess (P%d) somma = %d \n", column_index, sum);
    p->buffer[indice] = sum;
	p->stato[indice] = BUFFER_PIENO;
	Signal_Sem(ds_sem, SPAZIO_DISPONIBILE);
	Signal_Sem_Neg(ds_sem, MESSAGGIO_DISPONIBILE);	// ok?
}

// consumatore
void consumatore(struct prodcons * p, int ds_sem) {
	int sum = 0;	
	Wait_For_Zero_Sem(ds_sem, MESSAGGIO_DISPONIBILE);
	for(int z = 0; z<DIM_COLONNE; z++) {
        if(p->stato[z] == BUFFER_PIENO){
            sum += p->buffer[z];
        } else { perror("ERRORE cell vuota"); }
	}
	p->sum = sum;
	printf("La somma è = %d\n", p->sum);
	// rilascio il buffer
	for(int i=0; i<DIM_COLONNE; i++){
		p->stato[i] = BUFFER_VUOTO;
		Signal_Sem(ds_sem, SPAZIO_DISPONIBILE);
	}
}