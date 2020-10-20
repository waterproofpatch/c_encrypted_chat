/**
 * @file server.c
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

/* system includes */
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

/* installed includes */
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/x509.h>

/* custom includes */
#include "server_start.h"
#include "semaphore.h"
#include "logger.h"

/* semaphore for waiting until server has started. */
semaphore_t gSem;

void init_openssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl()
{
    EVP_cleanup();
}

SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *         ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Listen and accept clients
 *
 * @param args struct containing server configuration
 */
static void *serve(void *args)
{
    server_args_t *    pServerArgs = (server_args_t *)args;
    int                server_fd, new_socket, valread;
    struct sockaddr_in address;
    int                opt          = 1;
    int                addrlen      = sizeof(address);
    char               buffer[1024] = {0};
    char *             hello        = "Hello from server";

    SSL_CTX *ctx;
    SSL *    ssl;

    init_openssl();
    ctx = create_context();

    configure_context(ctx);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(pServerArgs->port);

    LOG_INFO("Listening on %d\n", address.sin_port);

    /* Forcefully attaching socket to the port 8080 */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /* start listening for clients */
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* now the server is up and listening, we can notify anyone waiting for such
     */
    semaphore_post(&gSem);

    /* accept a client */
    if ((new_socket = accept(
             server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) <
        0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, new_socket);

    if (SSL_accept(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
    }
    else
    {
        SSL_write(ssl, hello, strlen(hello));
    }

    // valread = read(new_socket, buffer, 1024);
    // LOG_INFO("%s\n", buffer);
    // send(new_socket, hello, strlen(hello), 0);
    // SSL_write(ssl, hello, strlen(hello));
    // LOG_INFO("Hello message sent\n");

    SSL_shutdown(ssl);
    SSL_free(ssl);

    SSL_CTX_free(ctx);
    cleanup_openssl();

    return NULL;
}

/**
 * @brief Start a server and return the server's pthread object.
 *
 * @param port to listen on.
 * @return pthread_t* of the server.
 * @return NULL if no memory could be allocated.
 */
pthread_t *server_start(unsigned short port)
{
    server_args_t serverArgs = {.port = port};

    pthread_t *thread_id = malloc(sizeof(thread_id));
    if (!thread_id)
    {
        LOG_INFO("Failed allocating thread_id\n");
        return NULL;
    }
    memset(thread_id, 0, sizeof(*thread_id));

    LOG_INFO("Starting server\n");

    /* init a sem as taken */
    semaphore_init(&gSem, 0);

    pthread_create(thread_id, NULL, serve, &serverArgs);

    /* wait for the server to start up... */
    semaphore_wait(&gSem);

    LOG_INFO("Server started...\n");
    return thread_id;
}