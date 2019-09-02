# Remote File Transfer in C using Stream Sockets

server.c : Responsible for serving clients by providing them with the requested file, assuming the file is readable and in the same   
           directory as server.c
           Client is able to request any type of files(e.g. .jpg, .mp3...etc). Server.c runs indefinitely unless binding fails. 
           
           server.c requires 1 command-line argument, the port number when running.
           
           
client.c : Request a file of any type from the server. If the file exists in the server's directory and is readable, the client saves a                copy of the file in the directory that contains client.c. Upon error or recieving the requested file, client.c terminates.
           
           client.c requires 4 command-line arguments, the Server IPv4 address (in dot notation), port number, requested filename, and 
           new filename, respectively.
           
tree.jpg : Strictly for testing purposes.

To run(using tree.jpg as an example):

1. Download all three files and compile server.c and client.c separately.
2. Open 2 terminals (one to act as the server and another to act as a client)
3. Run server.c in one terminal: (Assumes output file is called server)
   ./server <port num>
   
   Run client.c in the other terminal: (Assumes output file is called client)
   ./client <Server IPv4 Address in dot notation> <port num (same as the above)> tree.jpg tree_copy.jpg
   
   
Note: Requested file from client does not need to be in same directory as client.c
