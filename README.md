# QuickFT

The QuickFT protocol emerged from the need to transport files to and from embedded devices on
a LAN and delete files remotely. The original version was written for Windows, aimed at
circumventing the operating system's network file operations capabilities and implement its own.

Both the QuickFT protocol and the Linux port should be considered an excersise in protocol design
and sockets programming in C language.

The library implementation allows programs to set up a server or a client very easily, as shown in
the program examples bundled with the library.

The server is designed in such a way that allows serving multiple simultaneous requests from file
operations, up to a hard-coded max. of 256 (which could be parametrized easily if needed).

The library supports three types of operations, which from the client point of view would be:
transfer a file to the server, transfer a file from the server and delete a device from the server.

The protocol is limited to single file operations, not permitting multiple files to be affected by a
single action.
