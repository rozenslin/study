#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

// mutex
pthread_mutex_t lock;
static int id = 0;

// 1. Start 5 threads.
// 2. Each thread should print a unique thread ID number (1, 2, 3, 4, or 5).

// To execute C, please define "int main()"
void t_func()
{
    pthread_mutex_lock(&lock);
    fprintf(stderr, "thread id: %d\n", id); 

    id++;

    pthread_mutex_unlock(&lock);
}

void do_pthread()
{
    int i = 0;
    for (; i <= 4; i++)
    {
        pthread_t thread;
        int ret = pthread_create(&thread, NULL, (void*)t_func, NULL);
        if (!ret)
        {
            pthread_join(thread, NULL);
        }
    }
}

int main() {

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    do_pthread();

    pthread_mutex_destroy(&lock);

    return 0;
}
