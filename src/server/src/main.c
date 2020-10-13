/**
 * @file main.c
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

/* system includes */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* custom includes */
#include "server_start.h"
#include "logger.h"

int main(int argc, char const *argv[])
{
    pthread_t *serverThread = server_start(8080);
    if (!serverThread)
    {
        LOG_INFO("Failed starting server...\n");
        return 1;
    }

    LOG_INFO("waiting for server to finish...\n");
    pthread_join(*serverThread, NULL);
    LOG_INFO("Server stopped.\n");
    free(serverThread);
    serverThread = NULL;
    return 0;
}