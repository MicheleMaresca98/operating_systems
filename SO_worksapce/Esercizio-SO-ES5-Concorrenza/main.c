#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

int main() {
	/*
	 * Definizione della chiave
	*/

	// gestione dei dati sulla memoria condivisa
	key_t chiave = IPC_PRIVATE;
	int ds_shm = shmget(chiave, sizeof(struct prodcons), IPC_CREAT|0664);
	if(ds_shm<0) { perror("SHM errore"); exit(1); }
	struct prodcons * p;
	p = (struct prodcons *) shmat(ds_shm, NULL, 0);

	// iniziliazzazione del buffer di stato
	for(int i=0; i<DIM_COLONNE; i++) { p->stato[i] = BUFFER_VUOTO; }

	// inizializzo le variabili condivise
	p->sum = 0;
	printf("matrix printing \n");
	for(int i=0; i<DIM_RIGHE; i++){
		for(int j=0; j<DIM_COLONNE; j++){
			p->matrix[i][j] = i%2;
			printf(" | %d", i%2);
		}
		printf("\n");
	}

	// gestione dei semafori
	key_t chiavesem = IPC_PRIVATE;
	int ds_sem = semget(chiavesem, 4, IPC_CREAT|0664);
	if(ds_sem<0) { perror("SEM errore"); exit(1); }	
	semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, DIM_COLONNE);
	semctl(ds_sem, MESSAGGIO_DISPONIBILE, SETVAL, DIM_COLONNE);
	semctl(ds_sem, MUTEX_P, SETVAL, 1);

	/*
	 * Creazione del consumatore
	*/
	int pid_consumatore = fork();
	if(pid_consumatore==0) {
		printf("id filgio: %d, valore sem ass: %d", 
		getpid(), semctl(ds_sem, MESSAGGIO_DISPONIBILE, GETVAL,0));
		printf("Inizio figlio consumatore\n");
		consumatore(p, ds_sem);
		exit(1);
	} else { printf("Pid Consumatore %d \n", pid_consumatore); }

	/*
	 * Creazione dei produttori
	*/
	for(int i=0; i<DIM_COLONNE; i++) {
		int pid = fork();
		if(pid==0) {
			printf("Inizio figlio produttore\n");
			produttore(p, ds_sem, i);
			exit(1);
		}
	}

	/*
	 * Attendo la terminazione del consumatore
	 * 
	*/
	// pid consumatore non può terminare se i produttori
	// non hanno occupato tutto il buffer
	for(int i=0; i<=DIM_COLONNE; i++){
		wait(NULL);
	}

	// Terminazione del processo padre
	printf("Processo padre terminato terminato\n");
	return 0;
}
