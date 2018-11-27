/* $Id$
 * spop.c      c-source file for main server
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>

#include "network.h"
#include "pop3.h"
#include "smallpop3.h"

/* default name and port:
 * The port is given as a string as this is the form needed
 * for the getaddrinfo(3) function
 */
char defaultport[] = "65000";
char *port = defaultport;

/* useage string:
 * used as a help messge
 */
char help[] = "Usage: spop [-h] [-p port] \n"
		"\t-h\tshow this help\n"
		"\t-p port\tuse alternate port (default %s)\n";

/* Logfile:
 */
char logfile[] = "spop.log";
FILE *stdlog;

/* predeclare functions */
void accept_clients(int);

int main(int argc, char *argv[])
{
    int server;
    int o;
    /* option processing
     * The handling of the p option is fragile,
     * a robust solution would be to use strcpy(port,optarg)
     * this is because optarg gets reused each time getopt is called.
     */
    while( (o=getopt(argc,argv,"hp:"))!=-1)
    {
         switch(o){
         case 'h': printf(help,defaultport);exit(EXIT_SUCCESS);break;
         case 'p': port = optarg; break;
         }
    }
    fprintf(stderr,"starting\n");
    /* Set up the server on the port
    */
    server = makeTCPserver(port);
    accept_clients(server);
    exit(EXIT_SUCCESS);
}

/* This function handles the aceptance of clients.
 * It enters an infinite loop, calls accept, and forks for each new 
 * client connecting to the server.
 *
 * errors here cause error messages, but are not fatal to the server.
 */
void accept_clients(int s)
{
    int c;
    pid_t p;

    for(;;)
    {
        fprintf(stderr,"ready...\n");
        if( (c=accept(s,(struct sockaddr *)&client, &addrlen))==-1)
        {
            nonfatalerror(strerror(errno));
            continue;
        }
        fprintf(stderr,"client on %d\n",c);
        switch( p = fork())
        {
        case -1:
            nonfatalerror(strerror(errno));
            continue;
        case 0:
            /* child */
            handle_client(c);
            shutdown(c,SHUT_RDWR);
            close(c);
            exit(EXIT_SUCCESS);
            /* this exits the child process,
             * not the parent server
             */
            break;
        }
    }
}
