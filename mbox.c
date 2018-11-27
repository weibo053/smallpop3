/* $Id$
 * mbox.c      c-source file for mbox manipulation
 */

#include <stdio.h>
#include <string.h>
#include "mbox.h"
#include "network.h"
#include "smallpop3.h"

/* messages:
 * Use a fixed array is appropriate as this is for the lab exercises.
 */
struct mbox emails[MAX_MESSAGES];
int nemails = 0;
long tsize = 0;

/* scan mbox to count emails and find offsets.
 * An email begins with the string 'From ' after a blank line.
 */
int scanmbox(FILE* mb){
    char *line;
    int i;
    long pos;
    while( pos=ftell(mb),line=readline(mb) ){
        if(strncmp("From ",line, 5)==0 ){
            emails[nemails++].offset = pos;
        }
    }
    emails[nemails].offset = ftell(mb);
    for(i=0;i<nemails;++i)
    {
        emails[i].bytes = emails[i+1].offset-emails[i].offset;
        tsize += emails[i].bytes;
    }
    return nemails;
}
