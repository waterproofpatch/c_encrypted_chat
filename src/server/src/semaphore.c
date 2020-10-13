/**
 * @file semaphore.c
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

/* custom includs */
#include "semaphore.h"

/**
 * @brief Initialize a semaphore.
 *
 * @param pSem pointer to semaphore to initialize. Assumed non-NULL.
 * @param initialValue initial value the semaphore has.
 */
void semaphore_init(semaphore_t *pSem, uint32_t initialValue)
{
#ifdef __APPLE__
    dispatch_semaphore_t *sem = &pSem->sem;

    *sem = dispatch_semaphore_create(initialValue);
#else
    semaphore_init(&pSem->sem, 0, value);
#endif
}

/**
 * @brief Wait for a semaphore's value to increment to > 0.
 *
 * @param pSem pointer to semaphore to wait for. Assumed non-NULL.
 */
void semaphore_wait(semaphore_t *pSem)
{

#ifdef __APPLE__
    dispatch_semaphore_wait(pSem->sem, DISPATCH_TIME_FOREVER);
#else
    int r;

    do
    {
        r = semaphore_wait(&pSem->sem);
    } while (r == -1 && errno == EINTR);
#endif
}

/**
 * @brief Increment the semaphore.
 *
 * @param pSem pointer to semaphore to increment. Assumed non-NULL.
 */
void semaphore_post(semaphore_t *pSem)
{

#ifdef __APPLE__
    dispatch_semaphore_signal(pSem->sem);
#else
    semaphore_post(&pSem->sem);
#endif
}