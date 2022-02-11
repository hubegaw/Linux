#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>

int main()
{
int semID;
key_t klucz;
struct sembuf operacje[1];

if((klucz=ftok(".",'A')) == -1) {
	printf("Błąd ftok\n");
}
semID = semget(klucz, 1, IPC_CREAT | IPC_EXCL | 0666);
semctl(semID, 0, SETVAL, 0);

int pdesk[2];
pipe(pdesk);
switch (fork())
{
case -1:
   	exit(1);
case 0:
	operacje[0].sem_num = 0;
	operacje[0].sem_op = -1;
	operacje[0].sem_flg = 0;
	semop(semID, operacje, 1);
	semctl(semID, 0, IPC_RMID, NULL);

	close(1);
	dup(pdesk[1]);
	close(pdesk[0]);
	close(pdesk[1]);
	execlp("cat","cat","plik1","plik2",NULL);
default:
	switch (fork())
	{
	case -1:
		exit(1);
	case 0:
		close(0);
		dup(pdesk[0]);
		close(pdesk[0]);
		close(pdesk[1]);
		execlp("wc","wc",NULL);
		exit(1);
	default:
		printf("plik 1: \n");
		system("cat > plik1");

		printf("plik 2: \n");
		system("cat > plik2");
		
		operacje[0].sem_num = 0;
		operacje[0].sem_op = 1;
		semop(semID, operacje, 1);
		sleep(2);
	}
}
//semctl(semID, 0, IPC_RMID, NULL);
//potok nienazwany
//procesy: macierzysty i dwa procesy potomne

//można wykorzystać pliki operacje.c i operacje.h
//proces macierzysty: tworzy pliki:plik1 i plik2 - edycja użytkownka z klawiatury, podnosi semafor
//proces potomny1: czeka na podniesienie semafora, do potoku zapisuje
// oba pliki (cat + podmiana standardowego wyjścia)
//proces potomy2: odczytuje dane z tego potoku, wykonuje
// polecenie wc (+ podmiana standardowego wejścia); wynikwyświetla na ekranie

}
