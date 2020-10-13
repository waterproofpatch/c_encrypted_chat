/**
 * @file semaphore.h
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

/* system includes */
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

/**
 * @brief Semaphore
 *
 */
typedef struct semaphore
{
#ifdef __APPLE__
    dispatch_semaphore_t sem; /**< underlying primitive */
#else
    sem_t sem; /**< underlying primitive */
#endif
} semaphore_t;

void semaphore_init(semaphore_t *pSem, uint32_t initialValue);
void semaphore_wait(semaphore_t *pSem);
void semaphore_post(semaphore_t *pSem);