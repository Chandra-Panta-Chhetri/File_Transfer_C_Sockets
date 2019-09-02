#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFFERSIZE 4096

void error_handler(const char *error_msg)
{
  perror(error_msg);
  exit(1);
}


/* sends required info needed by server in order to get requested file from
   server */
int send_file_info(int cli_sockfd, char *argv[])
{
  int filename_len = strlen(argv[3]);
  int file_found;

  /* sending length of path to requested file */
  write(cli_sockfd, &filename_len, sizeof(int));
  /* sending filepath to server */
  write(cli_sockfd, argv[3], filename_len + 1);
  /* reading signal from server as to whether file found or not */
  read(cli_sockfd, &file_found, sizeof(int));

  /* protocol to execute if problems with requested file */
  if(file_found == 0) {
    close(cli_sockfd);
    error_handler("Filename provided not found or not readable.");
  }
  /* protocol to execute if no problems with requested file
     0666 indicates read and write permission to new file*/
  int newfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(newfile_fd == -1)
  {
    fprintf(stderr, "Opening file for writing error.\n");
    return -1;
  }
  return newfile_fd;
}


/* writes to file pointed by fp the contents of file sent by server */
void save_file(int cli_sockfd, int newfile_fd)
{
  char buf[BUFFERSIZE];
  int bytes_read;
  /*reading number of bytes server wrote to client socket and writing
    appropirate number of bytes to new file */
  while((bytes_read = read(cli_sockfd, buf, BUFFERSIZE)) > 0)
  {
    write(newfile_fd, buf, bytes_read);
  }
}

/* argv[1] = IPv4 address, argv[2] = port number, argv[3] = requested file path
   argv[4] = path to save recieved file from server */
int main(int argc, char *argv[])
{
  if(argc < 5) {
    error_handler("Please enter correct arguments.");
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));

  /* Checking if IPv4 address valid */
  if (0 == inet_pton(AF_INET, argv[1], &(serv_addr.sin_addr))) {
    error_handler("Invalid IPv4 address.");
  }

  int cli_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(cli_sockfd == -1)
  {
    error_handler("Socket Creation Error.");
  }
  if(connect(cli_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
  {
    error_handler("Connection Error.");
  }

  int newfile_fd = send_file_info(cli_sockfd, argv);
  if(newfile_fd != -1) {
    save_file(cli_sockfd, newfile_fd);
    close(newfile_fd);
  }

  close(cli_sockfd);
  return 0;
}
