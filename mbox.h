/* $Id$
 * mbox.h      header file for mbox manipulation
 * This file defines useful constants and structures for
 * the manipulationn of an mbox style mail folder.
 */

/* internal structures:
 * These are used to store information about the structure of the mbox
 * file in the server.  The offset and size of each message, the
 * offset for fseek(3) and the size for the POP3 protocol
 * We are never going to have many messages so a static array removes
 * any need for malloc et.el.
 */
struct mbox {
    long offset;
    size_t bytes;
};
#define MAX_MESSAGES 16

/* variables and functions to export:
 */
extern struct mbox emails[];
extern int nemails;
extern long tsize;
int scanmbox(FILE*);
