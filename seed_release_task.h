#ifndef SEED_RELEASE_H
#define SEED_RELEASE_H

#define SEED_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE
#define SEED_TASK_PRIORITY ( tskIDLE_PRIORITY + 2UL )

extern xSemaphoreHandle xSeedSemaphore;

void seed_release_task(__unused void *params);


#endif