#include <stdio.h>
#include <pthread.h>

int global_var = 0;
pthread_mutex_t mutexA;

void *inc_gv(void *ptr)
{
	char *str;
	str = (char *) ptr;
	int i, j;
	for (i=0;i<10;i++)
	{
        pthread_mutex_lock(&mutexA);
		global_var ++;
		for (j=0;j<5000000;j++);
		printf("%s:", str);
		printf(" %d", global_var);
        printf("\n");
		fflush(stdout);
         pthread_mutex_unlock(&mutexA);
	}
}

int main ()
{
	pthread_t threadA, threadB;
	int tAret, tBret;

	char * str1 = "[A]";
	char * str2 = "[B]" ;

    pthread_mutex_init(&mutexA, NULL);

//	tAret = pthread_create(&threadA, NULL, inc_gv, NULL);
//	tBret = pthread_create(&threadB, NULL, inc_gv, NULL);

	tAret = pthread_create(&threadA, NULL, inc_gv, (void *) str1 );	
	tBret = pthread_create(&threadB, NULL, inc_gv, (void *) str2);
	pthread_join(threadA, NULL);
	pthread_join(threadB, NULL);
	printf("finished\n");

	return 0;
}


