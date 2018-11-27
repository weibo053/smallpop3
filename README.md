# smallpop3
A cut down pop3 server for use in teaching protocols
----

# smallpop3

smallpop3 is a tiny pop3 server for illustrating the main features of
the pop3 protocol.  To use it you need three files, the server
`smallpop3` the user list `maillist` and a sample mailbox `mbox.mail`.

To run the server simply run `smallpop3` from the command prompt.
To stop the server use control-C (^C)

Ensure that

	1. all the files are in the same directory
	2. you have changed (cd) to that directory

_Note:_  
When the client sends a quit, the server correctly closes the socket.
However the telnet client used in the exercises does not exit until 
the server has been stopped.

any comments to alun.moon@unn.ac.uk

## Instructions

The POP3 protocol is used to read or download incoming messages from a
mail server to a users PC.  In theory it would be possible to connect
to the University's email server using telnet, but security issues make
this impractical.  As an alternative we can provide a small POP3 server
to allow you to interrogate a fictitious mailbox.  The small POP3
server does not implement all the commands that are described in
RFC1939, but some of the most used commands are implemented.  As with
any POP3 server, you need to log on before you can access the
emails.  The mailbox belongs to a fictitious user with id guest and
password visitor

The server can be set running on the linux workstation with the command
```sh
smallpop3
```

The program uses port 65000, and you can refer to the workstation with
the hostname localhost.  Now open another terminal window and use
telnet to connect to smallpop3 with the command:

```sh
telnet localhost 65000
```

As before we are using telnet to allow us to see the messages that
would normally be exchanged between an email client (such as Outlook)
and the pop3 server To log in to access the mail messages use the
commands

```
user guest
pass visitor
```

Notice that the POP3 server responds to commands with either +OK or
ERR to allow the client to detect if the command has worked or
contains an error.  You can use the LIST command to get a list of all
the messages currently in the mailbox.

LIST

Note that all you get is a numbered list of email sizes, but you can
retrieve message number 3, say, by using the command

RETR 3

Remember that you are now seeing the email message in its internal
format, with all of the headers showing how the message has been
routed through the email system. One of the jobs of a mail client such
as Outlook is to format and display the message in a way that is
better suited to the human reader.

Finally use the QUIT command to exit your pop3 session.

