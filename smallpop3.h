/* $Id$
 * smallpop3.h      header file for main server
 * This file defines useful constants and structures for
 * the main server program.
 */

/* error and diagnostic reporting:
 * use macros so that the __LINE__ directive expand to the line
 * number where the action caused the error to be reported.
 * Errors and diagnostic messages are printed on standard error.
 */
#define nonfatalerror(m) fprintf(stderr,__FILE__":%d %s\n",__LINE__,m)
#define diagnostic(m) nonfatalerror(m)

#define FATALerror(m) {nonfatalerror(m); exit(EXIT_FAILURE);}

extern FILE *stdlog;
