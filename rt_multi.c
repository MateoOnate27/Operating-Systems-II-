#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>

struct thread_config {
    int id;
    int priority;
    long period_ms;
};

void* rt_thread_func(void* arg) {
    struct thread_config *config = (struct thread_config*)arg;
    struct sched_param param;
    param.sched_priority = config->priority;

    // Setting FIFO scheduler for this thread
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler failed");
        return NULL;
    }

    struct timespec next;
    clock_gettime(CLOCK_MONOTONIC, &next);

    while(1) {
        // Calculate next activation
        next.tv_nsec += config->period_ms * 1000000;
        while (next.tv_nsec >= 1000000000) {
            next.tv_nsec -= 1000000000;
            next.tv_sec++;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
        printf("Thread T%d (Prio %d) executed at %ld.%09ld\n", 
               config->id, config->priority, next.tv_sec, next.tv_nsec);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    // T1: 90 Prio, 1ms period | T2: 70 Prio, 5ms period | T3: 50 Prio, 10ms period [cite: 164-169]
    struct thread_config c1 = {1, 90, 1};
    struct thread_config c2 = {2, 70, 5};
    struct thread_config c3 = {3, 50, 10};

    printf("Starting Multi-Task RT Program. Press CTRL+C to stop.\n");

    pthread_create(&t1, NULL, rt_thread_func, &c1);
    pthread_create(&t2, NULL, rt_thread_func, &c2);
    pthread_create(&t3, NULL, rt_thread_func, &c3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}
