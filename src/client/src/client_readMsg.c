/**
 * @file client_readMsg.c
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

/* system includes */
#include <stdio.h>
#include <unistd.h>

/* installed includes */
#include <openssl/ssl.h>

/* custom includes */
#include "client_readMsg.h"
#include "logger.h"

/**
 * @brief Read a message from the remote socket.
 *
 * @param sock socket to read message from.
 * @param[out] msg msg to read.
 * @param msgLen length of the msg to read.
 * @return number of bytes read on success.
 * @return -1 on failure.
 */
int client_readMsg(SSL *ssl, char *msg, size_t msgLen)
{
    int ret = 0;
    LOG_DEBUG("Reading message length %ld\n", msgLen);
    ret = SSL_read(ssl, msg, msgLen);
    LOG_DEBUG("Message read\n");
    return ret;
}