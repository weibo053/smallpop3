/* $Id$
 * network.c	c-source for network support
 */
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "smallpop3.h"
#include "network.h"

struct addrinfo hints = {
    AI_PASSIVE,
    PF_INET,
    SOCK_STREAM,
    0, 0, NULL
};

struct addrinfo *results;
struct sockaddr_in client;
socklen_t addrlen = sizeof(client);

/* this function creates a server on a TCP port.
 * it creates the socket, binds it to a port and makes
 * it listen for incomming connections.
 *
 * The parameter is a string containing the port number to use,
 * it returns the descriptor referencing the socket
 *
 * A failure at this stage is fatal and causes the server to exit abnormally.
 */
int makeTCPserver(char *port)
{
    int tcp_socket;
    /* create a ip4 tcp socket */
    if ((tcp_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        FATALerror(strerror(errno));

    /* get the address information needed to bind it to a port */
    if ((errno = getaddrinfo(NULL, port, &hints, &results)))
        FATALerror(gai_strerror(errno));

    if (bind(tcp_socket, results->ai_addr, results->ai_addrlen))
        FATALerror(strerror(errno));

    if (listen(tcp_socket, 2))
        FATALerror(strerror(errno));

    return tcp_socket;
}

/* Data and functions for readline
 */
#define LINELENGTH 132
char linebuffer[LINELENGTH];
/* Readline is a simple wrapper around fgets.
 * It fills in the buffer pointer and length
 */
char *readline(FILE * stream)
{
    return fgets(linebuffer, LINELENGTH, stream);
}
