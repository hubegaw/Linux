#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <semaphore.h>

struct kolejka {
	long mtype;
	int mvalue;
} komunikat;

#define MAX 10
#define MAX2 12
#define PELNY 2
#define PUSTY 1
#define odczyt pam[MAX]
#define zapis pam[MAX+1]

int *pam;

int main() {
	
	int i=0;
	int msgID, shmID, semID;
	key_t kluczk, kluczs, kluczp;

	if((kluczk = ftok(".", 'A')) == -1) {
		printf("Błąd tfok (A)\n");
		exit(2);
	}
	
	kluczp=ftok(".",'B');
	kluczs=ftok(".",'C');

	if((msgID = msgget(kluczk, IPC_CREAT | 0666)) == -1) {
		printf("Błąd kolejki komunikatów\n");
		exit(1);
	}

	shmID=shmget(kluczp, MAX2*sizeof(int), IPC_CREAT | 0666);
	pam=(int*)shmat(shmID, NULL, 0);
	
	if((semID = semget(kluczs, 1, IPC_CREAT | 0666)) == -1) {
		printf("Błąd semafora\n");
		exit(1);
	}

	if(msgrcv(msgID, &komunikat, sizeof(komunikat.mvalue), 0, PUSTY) == -1) {
		printf("Błąd otrzymania komunikatu\n");
		exit(1);
	}

	sleep(getpid()%10);
	
	struct sembuf operacje;

	operacje.sem_num = 0;
	operacje.sem_op = -1;
	semop(semID, &operacje, 1);

	pam[zapis] = getpid();
	printf("\nPID=%d, zapisałem pod bufor %d : %d\n", pam[zapis], zapis, pam[zapis]);

	zapis++;
	if((zapis%MAX) == 0) {
		zapis = 0;
	}
	
	operacje.sem_num = 0;
	operacje.sem_op = 1;
	semop(semID, &operacje, 1);

	if(msgsnd(msgID, &komunikat, sizeof(komunikat.mvalue), PELNY) == -1) {
		printf("Błąd wysłania komunikatu\n");
		exit(1);
	}
}
