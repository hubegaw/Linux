#include <stdio.h>
#include "operacje.h"
#include <sys/types.h>
#include <unistd.h>

int main()
{
   key_t klucz;
   int semID;
   int N = 3;

   if ( (klucz = ftok(".", 'A')) == -1 )
   {
      printf("Blad ftok (A)\n");
      exit(2);
   }

   semID = alokujSemafor(klucz, N, IPC_CREAT | 0666);

   printf("wykonuje a1\n");
   sleep(1);
   signalSemafor(semID, 0);

   printf("wykonuje a2\n");
   sleep(1);
   signalSemafor(semID, 2);
}

