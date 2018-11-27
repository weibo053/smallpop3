# makefile for smallpop3

SERVER=smallpop3.c mbox.c pop3.c network.c
HEADERS=smallpop3.h mbox.h pop3.h network.h

EXTRAS=maillist mbox.mail

all: smallpop3 smallpop3.tar.gz

# software
smallpop3: $(SERVER) $(HEADERS)
	cc -o smallpop3 $(SERVER)

smallpop3.tar.gz: smallpop3 $(EXTRAS)
	tar czf smallpop3.tar.gz smallpop3 $(EXTRAS)

.PHONY:clean
clean:
	rm smallpop3 *.o


