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

/* installed includes */
#include <openssl/ssl.h>

#include "client_start.h"
#include "client_readMsg.h"
#include "client_sendMsg.h"
#include "logger.h"

int main(int argc, char const *argv[])
{
    char *hello        = "Hello from client";
    char  buffer[1024] = {0};

    LOG_INFO("Starting client\n");
    SSL *ssl = client_start("127.0.0.1", 8080);
    if (ssl == NULL)
    {
        LOG_ERROR("Failed getting socket.\n");
        return 1;
    }
    assert(client_sendMsg(ssl, hello, strlen(hello)) == 0);
    assert(client_readMsg(ssl, buffer, 1024) == 0);
    LOG_INFO("Read: %s\n", buffer);
}