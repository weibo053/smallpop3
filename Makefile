# makefile for smallpop3

SERVER=smallpop3.c mbox.c pop3.c network.c
HEADERS=smallpop3.h mbox.h pop3.h network.h

EXTRAS=maillist mbox.mail

smallpop3: $(SERVER) $(HEADERS)
	cc -o smallpop3 $(SERVER)

.PHONY:clean
clean:
	rm -f smallpop3 *.o


