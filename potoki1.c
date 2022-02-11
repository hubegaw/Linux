#include <stdio.h>
//#include <errno.h>
//#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{

int pdesk[2];
int pdesk2[2];
pipe(pdesk);
switch (fork())
{
case -1:
  exit(1);
case 0:
	pipe(pdesk2);
	switch (fork())
	{
	case -1:
		exit(1);
	case 0:
		close(1);
		dup(pdesk2[1]);
		close(pdesk2[0]);
		close(pdesk2[1]);
		execlp("cat","cat","potok2.c", NULL);
	default:
	close(1);
	dup(pdesk[1]);
	close(pdesk[0]);
	close(pdesk[1]);

	close(0);
	dup(pdesk2[0]);
	close(pdesk2[0]);
	close(pdesk2[1]);
	execlp("grep","grep","open",NULL);
	}
default:
  close(0);
  dup(pdesk[0]);
  close(pdesk[0]);
  close(pdesk[1]);
  execlp("cat","cat","-n",NULL);
}
}
