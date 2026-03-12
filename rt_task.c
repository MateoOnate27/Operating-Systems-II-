#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>

#define PERIOD_NS 1000000 // Periodo de 1ms

int main() {
    struct sched_param param;
    param.sched_priority = 80; // Prioridad alta [cite: 394]
    
    // Establecer el planificador FIFO para tiempo real [cite: 395]
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler failed");
        return 1;
    }

    struct timespec next, current;
    clock_gettime(CLOCK_MONOTONIC, &next); // Tiempo inicial [cite: 397]

    printf("Starting Real-Time Task. Press CTRL+C to stop.\n");

    while(1) {
        // Calcular el próximo tiempo de ejecución [cite: 400]
        next.tv_nsec += PERIOD_NS;
        if(next.tv_nsec >= 1000000000) {
            next.tv_nsec -= 1000000000;
            next.tv_sec++;
        }

        // Dormir hasta el tiempo absoluto programado [cite: 407]
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);

        // Obtener el tiempo actual para calcular el jitter [cite: 423-424]
        clock_gettime(CLOCK_MONOTONIC, &current);
        
        // Calcular la desviación (jitter) [cite: 425-427]
        long diff = (current.tv_nsec - next.tv_nsec);
        
        // Si el segundo cambió, ajustamos el cálculo del diff
        if (current.tv_sec > next.tv_sec) {
            diff += 1000000000;
        }

        printf("Task executed. Jitter: %ld ns\n", diff);
    }
    return 0;
}
