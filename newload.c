#include <stdio.h>
#include <math.h>
#include <sys/mman.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>

#define NTHREADS 30

#define SIZE 1000000

void *func(void *arg)
{
   int i, j, rcode;
   double *a;

   rcode = mlockall(MCL_CURRENT | MCL_FUTURE);
   if (rcode !=0 ) {
      printf("Problem with mlockall\n");
   }

   for (j = 0; ; j++) {
    a = (double *)calloc(SIZE, sizeof(double));
   	for (i = 0; i < SIZE; i++) {
      	printf("Simulating a load ... Count = %ld\n", i);
      	a[i] = sqrt((double) i);
   	}
   	free(a);
   }

   munlockall();
}

int main(void)
{
	pthread_t loads[NTHREADS];
	int i;

	for (i = 0; i < NTHREADS; i++) {
		if (pthread_create(&loads[i], NULL, &func, &i) != 0) {
			printf("Error creating thread %d\n", i);
			exit(-1);
		}
		else {
			printf("Created Thread %d\n", i);
		}
	}

	pthread_exit(NULL);
}
