#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1
#define MUTEX_P 2

#define DIM_COLONNE 5
#define DIM_RIGHE 3

#define BUFFER_VUOTO 0
#define BUFFER_INUSO 1
#define BUFFER_PIENO 2


struct prodcons {
    int matrix[DIM_RIGHE][DIM_COLONNE];
	int buffer[DIM_COLONNE];
	int stato[DIM_COLONNE];
    int sum;
};

// procedures 
void Wait_Sem(int semid, int semnum);
void Wait_For_Zero_Sem(int semid, int semnum);
void Signal_Sem(int semid, int semnum);
void produttore(struct prodcons * p, int ds_sem, int column_index);
void consumatore(struct prodcons * p, int ds_sem);