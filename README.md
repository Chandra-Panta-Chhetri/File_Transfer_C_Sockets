# Remote File Transfer in C using Stream Sockets

server.c : Responsible for serving clients by providing them with the requested file, assuming the file is readable by the server.c. Client            is able to request any type of files(e.g. .jpg, .mp3...etc). Server.c runs indefinitely unless 
           binding fails. 
           
           server.c requires 1 command-line argument, the port number when running.
           
           
client.c : Request a file of any type from the server. If the file is readable by server, the client saves a copy of the file in the 
           directory that contains client.c. Upon error or recieving the requested file, client.c            terminates.
           
           client.c requires 4 command-line arguments, the Server IPv4 address (in dot notation), port number, requested filename path, and 
           new filename path, respectively.
           
           Note: If a file with the same name as the name specified in the 4th command-line argument of client.c, the file will be  
                 overwritten, not appeneded to.
           
tree.jpg : Strictly for testing purposes.

To run(using tree.jpg as an example): (Assume the following path exists)

1. Download all three files and compile server.c and client.c separately.
2. Open 2 terminals (one to act as the server and another to act as a client)
3. Run server.c in one terminal: (Assumes output file is called server)
   ./server <port num>
   
   Run client.c in the other terminal: (Assumes output file is called client)
   ./client <Server IPv4 Address in dot notation> <port num (same as the above)> "./test_files/tree.jpg" "./test_output/tree_copy.jpg"
   
   
Note: If requested file exist in current directory, specify only the filename as the 3rd command-line argument of   
      client.c. 
      If client would like a copy of the requested file in current directory, specify only the filename as the 4th command-line argument of       client.c.
