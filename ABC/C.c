#include <stdio.h>
#include "operacje.h"
#include <sys/types.h>
#include <unistd.h>

int main() 
{

key_t klucz;
int semID;
int N=4;

if ( (klucz = ftok(".", 'A')) == -1 )
{
printf("Blad ftok (C)\n");
exit(2);
}

semID = alokujSemafor(klucz, N, IPC_CREAT | 0666);

printf("wykonuje c1\n");
sleep(3);
signalSemafor(semID, 1);

printf("wykonuje c2\n");
sleep(3);

wait(semID, 3, 0);
printf("wykonuje c3\n");
sleep(3);

}
