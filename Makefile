# makefile for smallpop3

SERVER=smallpop3.c mbox.c pop3.c network.c
HEADERS=smallpop3.h mbox.h pop3.h network.h

# software
smallpop3: $(SERVER) $(HEADERS)
	cc -o smallpop3 $(SERVER)

clean:
	rm smallpop3 *.o
