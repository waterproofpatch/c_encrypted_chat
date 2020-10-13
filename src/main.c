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
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

/* custom includes */
#include "server_start.h"
#include "client_start.h"
#include "client_sendMsg.h"
#include "client_readMsg.h"
#include "logger.h"

int main(int argc, char const *argv[])
{
    pthread_t *serverThread = server_start(8080);
    char       buffer[1024];
    int        sock = -1;

    if (!serverThread)
    {
        LOG_ERROR("Failed starting server...\n");
        return 1;
    }

    LOG_INFO("Starting client\n");
    sock = client_start("127.0.0.1", 8080);
    assert(sock > 0);
    assert(client_sendMsg(sock, "Hey!", strlen("Hey!")) > 0);
    assert(client_readMsg(sock, buffer, sizeof(*buffer)) > 0);

    LOG_INFO("waiting for server to finish...\n");
    pthread_join(*serverThread, NULL);
    LOG_INFO("Server stopped.\n");
    free(serverThread);
    serverThread = NULL;
    return 0;
}