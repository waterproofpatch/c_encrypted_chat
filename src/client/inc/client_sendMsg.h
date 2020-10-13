/**
 * @file client_sendMsg.h
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _CLIENT_SENDMSG_H_
#define _CLIENT_SENDMSG_H_

/* system includes */
#include <stddef.h>

int client_sendMsg(int sock, char *msg, size_t msgLen);

#endif