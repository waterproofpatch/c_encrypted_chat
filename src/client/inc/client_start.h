/**
 * @file client_start.h
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _CLIENT_START_H_
#define _CLIENT_START_H_

/* installed includes */
#include <openssl/ssl.h>

SSL *client_start(char *hostname, unsigned short port);

#endif