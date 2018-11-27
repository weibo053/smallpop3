/* $Id$
 * pop3.c      c-source file for pop3 implementation
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "smallpop3.h"
#include "network.h"
#include "mbox.h"

FILE *cl, *mbox;

enum states {
    EXIT = 0,
    AUTHORIZATION,
    TRANSACTION
};
enum states serverstate;

int pop3_fsm(char *);
char *validate_user(char *, char *);

/* handle a pop3 client connected on socket s
 */
void handle_client(int s)
{
    char *inputline;

    if (!(cl = fdopen(s, "w+")))
        FATALerror(strerror(errno));
    fprintf(cl, "+OK server ready\n");
    serverstate = AUTHORIZATION;

    /* Read the following as:
     * while there is a line from input and the state-machine is
     * active, continue processing
     */
    while ((inputline = readline(cl)) && pop3_fsm(inputline))
        continue;
    fclose(cl);
}

/* functions for each of the pop3 commands.
 */
static char username[80];
static char mailbox[24];
char *mboxname;
static char *param;

int pop3user(void)
{
    param = strtok(NULL, " \t\r\n");
    if (!param) {
        fprintf(cl, "-ERR no usename\n");
        return AUTHORIZATION;
    }
    strcpy(username, param);
    fprintf(cl, "+OK\n");
    return AUTHORIZATION;
}

int pop3pass(void)
{
    param = strtok(NULL, " \t\r\n");
    if (!param) {
        fprintf(cl, "-ERR no password\n");
        return AUTHORIZATION;
    }
    mboxname = validate_user(username, param);
    if (!mboxname) {
        fprintf(cl, "-ERR user not authenticated\n");
        return AUTHORIZATION;
    }
    mbox = fopen(mboxname, "r");
    if (!mbox)
        FATALerror("Cannot open mailbox\n");
    scanmbox(mbox);
    fprintf(cl, "+OK\n");
    return TRANSACTION;
}

int pop3quit(void)
{
    fprintf(cl, "+OK pop3 server signing off\n");
    fclose(mbox);
    return EXIT;
}

int pop3stat(void)
{
    fprintf(cl, "+OK %i %li\n", nemails, tsize);
    return TRANSACTION;
}

int pop3list(void)
{
    param = strtok(NULL, " \t\r\n");
    if (param) {
        int m = atoi(param);
        if (m <= nemails) {
            fprintf(cl, "+OK %i %i\n", m, emails[m - 1].bytes);
        } else {
            fprintf(cl, "-ERR no such messages, only %i in maildrop\n",
                    nemails);
        }
    } else {
        int i;
        fprintf(cl, "+OK %i messages (%li octets)\n", nemails, tsize);
        for (i = 0; i < nemails; ++i)
            fprintf(cl, "%i %i\n", i + 1, emails[i].bytes);
        fprintf(cl, ".\n");
    }
    return TRANSACTION;
}

int pop3retr(void)
{
    param = strtok(NULL, " \t\r\n");
    if (!param) {
        fprintf(cl, "-ERR no such messagen\n");
    } else {
        int n = atoi(param);
        if (0 < n && n <= nemails) {
            fprintf(cl, "+OK %i octets\n", emails[n - 1].bytes);
            fseek(mbox, emails[n - 1].offset, SEEK_SET);
            while (ftell(mbox) < emails[n].offset) {
                fprintf(cl, readline(mbox));
            }
            fprintf(cl, ".\n");
        } else {
            fprintf(cl, "-ERR no such messagen\n");
        }
    }
    return TRANSACTION;
}

int pop3top(void)
{
    int m, n;
    param = strtok(NULL, " \t\r\n");
    if (!param) {
        fprintf(cl, "-ERR no such message\n");
    } else {
        m = atoi(param);
        param = strtok(NULL, " \t\r\n");
        if (!param) {
            fprintf(cl, "-ERR no size\n");
        } else {
            int l = 0, i = 0;
            n = atoi(param);
            fprintf(stderr, "(%s,%i)\n", param, n);
            if (0 < m && m <= nemails) {
                fprintf(cl, "+OK\n");
                fseek(mbox, emails[m - 1].offset, SEEK_SET);
                while (l < n && ftell(mbox) < emails[m].offset) {
                    char *line = readline(mbox);
                    fprintf(cl, line);
                    l += i;
                    if (strlen(line) < 2)
                        i = 1;
                }
                fprintf(cl, ".\n");
            } else {
                fprintf(cl, "-ERR no such message\n");
            }
        }
    }
    return TRANSACTION;
}

/* POP3 Finite State Machine
 * Takes an input string and returns the state of the machine;
 */
int pop3_fsm(char *input)
{
    char *cmd;
    cmd = strtok(input, " \t\r\n");
    if (cmd) {
        switch (serverstate) {
        case EXIT:
            return EXIT;
        case AUTHORIZATION:
            if (strcasecmp(cmd, "USER") == 0)
                return serverstate = pop3user();
            if (strcasecmp(cmd, "PASS") == 0)
                return serverstate = pop3pass();
            if (strcasecmp(cmd, "QUIT") == 0)
                return serverstate = pop3quit();
            break;
        case TRANSACTION:
            if (strcasecmp(cmd, "STAT") == 0)
                return serverstate = pop3stat();
            if (strcasecmp(cmd, "LIST") == 0)
                return serverstate = pop3list();
            if (strcasecmp(cmd, "RETR") == 0)
                return serverstate = pop3retr();
            if (strcasecmp(cmd, "TOP") == 0)
                return serverstate = pop3top();
            if (strcasecmp(cmd, "QUIT") == 0)
                return serverstate = pop3quit();
            break;
        }
    }
    fprintf(cl, "-ERR\n");
    return serverstate;
}

/*
 * Validate a user
 */
char *validate_user(char *name, char *pass)
{
    char uname[24], passwd[24];
    FILE *p;
    p = fopen("maillist", "r");
    if (!p)
        FATALerror("Cannot open maillist\n");
    while (fscanf(p, "%s %s %s", uname, passwd, mailbox) != EOF) {
        if ((strcmp(uname, name) == 0) && (strcmp(passwd, pass) == 0))
            return mailbox;
    }
    return NULL;
}
