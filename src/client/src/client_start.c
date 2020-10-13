/**
 * @file client_start.c
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
#include <arpa/inet.h>
#include <unistd.h> /* struct timeval */
#include <string.h>

/* custom includes */
#include "client_start.h"
#include "client_sendMsg.h"
#include "client_readMsg.h"
#include "logger.h"

/**
 * @brief Connect a client to a @c hostname: @c port tuple.
 *
 * @param hostname to connect to.
 * @param port to connect on.
 * @return nonzero on success, the socket file descriptor.
 * @return 0 on failure.
 */
int client_start(char *hostname, unsigned short port)
{
    int                sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG_INFO("\n Socket creation error \n");
        return 0;
    }

    struct timeval timeout;
    timeout.tv_sec  = 10;
    timeout.tv_usec = 0;

    /* set the receive timeout */
    if (setsockopt(
            sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) <
        0)
    {
        perror("setsockopt failed\n");
        return 0;
    }

    /* set the send timeout */
    if (setsockopt(
            sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) <
        0)
    {
        perror("setsockopt failed\n");
        return 0;
    }

    /* we're using INET sockets */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(port);
    LOG_INFO("Connecting on %d\n", serv_addr.sin_port);

    /* convert IPv4 and IPv6 addresses from text to binary form. */
    if (inet_pton(AF_INET, hostname, &serv_addr.sin_addr) <= 0)
    {
        LOG_INFO("\nInvalid address/Address not supported \n");
        return 0;
    }

    /* attempt a TCP connection to remote endpoint */
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        LOG_INFO("\nConnection Failed \n");
        return 0;
    }

    return sock;
}
