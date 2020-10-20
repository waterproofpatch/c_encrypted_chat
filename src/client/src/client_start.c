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

/* installed includes */
#include <openssl/ssl.h>
#include <openssl/err.h>

/* custom includes */
#include "client_start.h"
#include "client_sendMsg.h"
#include "client_readMsg.h"
#include "logger.h"

SSL_CTX *InitCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *         ctx;
    OpenSSL_add_all_algorithms(); /* Load cryptos, et.al. */
    SSL_load_error_strings();     /* Bring in and register error messages */
    method = TLS_client_method(); /* Create new client-method instance */
    ctx    = SSL_CTX_new(method); /* Create new context */
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void ShowCerts(SSL *ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if (cert != NULL)
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line); /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);      /* free the malloc'ed string */
        X509_free(cert); /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
}

/**
 * @brief Connect a client to a @c hostname: @c port tuple.
 *
 * @param hostname to connect to.
 * @param port to connect on.
 * @return SSL* context.
 * @return NULL on failure.
 */
SSL *client_start(char *hostname, unsigned short port)
{
    int                sock = 0;
    struct sockaddr_in serv_addr;
    SSL_CTX *          ctx;
    SSL *              ssl;

    SSL_library_init();
    ctx = InitCTX();

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
        return NULL;
    }

    /* set the send timeout */
    if (setsockopt(
            sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) <
        0)
    {
        perror("setsockopt failed\n");
        return NULL;
    }

    /* we're using INET sockets */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(port);
    LOG_INFO("Connecting on %d\n", serv_addr.sin_port);

    /* convert IPv4 and IPv6 addresses from text to binary form. */
    if (inet_pton(AF_INET, hostname, &serv_addr.sin_addr) <= 0)
    {
        LOG_INFO("\nInvalid address/Address not supported \n");
        return NULL;
    }

    /* attempt a TCP connection to remote endpoint */
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        LOG_INFO("\nConnection Failed \n");
        return NULL;
    }

    ssl = SSL_new(ctx);    /* create new SSL connection state */
    SSL_set_fd(ssl, sock); /* attach the socket descriptor */
    /* perform the connection */
    if (SSL_connect(ssl) == -1)
    {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    return ssl;
}
