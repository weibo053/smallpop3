/* $Id$
 * network.h	header file for network support
 */
#include <sys/types.h>
#include <sys/socket.h>

#ifdef OSF1
typedef int socklen_t;
#endif

extern struct sockaddr_in client;
extern socklen_t addrlen;

int makeTCPserver(char *);
char *readline(FILE *);
