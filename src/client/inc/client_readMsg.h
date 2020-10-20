/**
 * @file client_readMsg.h
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _CLIENT_READMSG_H_
#define _CLIENT_READMSG_H_

/* system includes */
#include <stddef.h>

/* installed includes */
#include <openssl/ssl.h>

int client_readMsg(SSL *ssl, char *msg, size_t msgLen);

#endif