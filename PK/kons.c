#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>

struct bufor {
	long mtype;
	int mvalue;
};

#define MAX 10
#define MAX2 12
#define PELNY 2
#define PUSTY 1
#define odczyt pam[MAX]
#define zapis pam[MAX+1]

int *pam;

int main() {
	
	int i;
	int msgID, shmID, semID;
	key_t kluczk, kluczs, kluczp;
	struct bufor komunikat;
	
	sleep(1);
	printf("konsument-------------------------------\n");

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

	if((semID = semget(semID, 1, IPC_CREAT | 0666)) == -1) {
		printf("Błąd semafora\n");
		exit(1);
	}

	if(msgrcv(msgID, &komunikat, sizeof(komunikat.mvalue), 0, PELNY) == -1) {
		printf("Błąd otrzymania komunikatu\n");
		exit(1);
	}
	printf("\nOdebrałem komunikat PELNY\n");

	struct sembuf operacje;
	
	operacje.sem_num = 0;
	operacje.sem_op = -1;
	semop(semID, &operacje, 1);
	
	sleep(1);
	printf("Konsument %d - odczyt z bufora %d :  %d\n",getpid(), odczyt, pam[odczyt]);
	
	odczyt++;
	if((odczyt%MAX) == 0) {
		odczyt = 0;
	}

	operacje.sem_num = 0;
	operacje.sem_op = 1;
	semop(semID, &operacje, 1);

	if(msgsnd(msgID, &komunikat, sizeof(komunikat.mvalue), PUSTY) == -1) {
		printf("Błąd wysłania komunikatu\n");
		exit(1);
	}
}
