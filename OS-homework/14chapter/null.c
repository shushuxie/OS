#include <pthread.h>
#include <stdio.h>
void
mythread(void *arg)
{
    printf("%s\n",arg);
}

int main() {
    pthread_t t1;
    int rc;
    pthread_create(&t1,NULL,mythread,NULL);
    pthread_join(rc,NULL);
	return 0;
}

