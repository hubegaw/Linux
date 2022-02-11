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
printf("Blad ftok (B)\n");
exit(2);
}

semID = alokujSemafor(klucz, N, IPC_CREAT | 0666);

wait(semID, 0, 0);
printf("wykonuje b1\n");
sleep(2);

wait(semID, 1, 0);
printf("wykonuje b2\n");
sleep(2);

wait(semID, 2, 0);
printf("wykonuje b3\n");
sleep(2);
signalSemafor(semID, 3);

}
