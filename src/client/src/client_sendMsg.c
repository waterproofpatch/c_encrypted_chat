/**
 * @file client_sendMsg.c
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
#include <sys/socket.h>

/* custom includes */
#include "client_sendMsg.h"
#include "logger.h"

/**
 * @brief Send a message to the remote socket.
 *
 * @param sock socket to send message to.
 * @param msg msg to send.
 * @param msgLen length of the msg to send.
 * @return number of bytes sent on success.
 * @return -1 on failure.
 */
int client_sendMsg(int sock, char *msg, size_t msgLen)
{
    int ret = 0;
    LOG_DEBUG("Sending message length %ld: %s\n", msgLen, msg);
    ret = send(sock, msg, msgLen, 0);
    LOG_DEBUG("Message sent.\n");
    return ret;
}