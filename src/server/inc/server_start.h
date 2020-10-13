/**
 * @file server_start.h
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _SERVER_START_H_
#define _SERVER_START_H_

/* system includes */
#include <pthread.h>

/**
 * @brief Argument struct to pass to the server.
 *
 */
typedef struct ServerArgs
{
    unsigned short port; /**< port to listen on. */
} server_args_t;

pthread_t *server_start(unsigned short port);

#endif