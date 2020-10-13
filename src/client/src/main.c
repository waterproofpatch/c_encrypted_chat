/* custom includes */
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

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "client_start.h"
#include "client_readMsg.h"
#include "client_sendMsg.h"
#include "logger.h"

int main(int argc, char const *argv[])
{
    char *hello        = "Hello from client";
    char  buffer[1024] = {0};

    LOG_INFO("Starting client\n");
    int sock = client_start("127.0.0.1", 8080);
    if (sock == 0)
    {
        LOG_ERROR("Failed getting socket.\n");
        return 1;
    }
    assert(client_sendMsg(sock, hello, strlen(hello)) == 0);
    assert(client_readMsg(sock, buffer, 1024) == 0);
    LOG_INFO("%s\n", buffer);
}