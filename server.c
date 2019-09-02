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


/* gets file path to requested file from client and returns a pointer to it,
   if successfully */
int get_file_info(int cli_sockfd)
{
  int filepath_len, file_found = 0;
  char *filepath = NULL;

  /* getting length of filepath from client */
  read(cli_sockfd, &filepath_len, sizeof(int));
  /* allocating enough memory to store requested filepath */
  filepath = malloc((filepath_len + 1) * sizeof(char));
  /* getting path to requested filepath from client*/
  read(cli_sockfd, filepath, (filepath_len + 1) * sizeof(char));

  int file_fd = open(filepath, O_RDONLY);
  if(file_fd == -1) {
    /* Telling client file specified by filepath cannot be read*/
    write(cli_sockfd, &file_found, sizeof(int));
    perror("File cannot be opened for reading.");
    free(filepath);
    return -1;
  }
  return file_fd;
}


/* Reads from requested file and sends its content to client */
void send_file(int cli_sockfd, int file_fd)
{
  int bytes_read;
  char buffer[BUFFERSIZE];

  /* Telling client that file was found and can be read */
  int file_found = 1;
  write(cli_sockfd, &file_found, sizeof(int));

  while((bytes_read = read(file_fd, buffer, BUFFERSIZE)) > 0)
  {
    /* sending read content from file to client */
    write(cli_sockfd, buffer, bytes_read);
  }
  fprintf(stderr, "File sent successfully.\n");
}


int main(int argc, char *argv[])  /* argv[1] = port number */
{
  if(argc < 2) {
    error_handler("Missing Port Number");
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, 0); /* sockfd is listening socket */
  if(sockfd == -1) {
    error_handler("Socket Creation Error");
  }

  struct sockaddr_in serv_addr;                       /* listening address */
  memset(&serv_addr, 0, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handler("Binding Error");
  }
  if(listen(sockfd, 2) == -1) {
    error_handler("Listening Error");
  }

  for(;;) /* serving clients indefinitely */
  {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int cli_sockfd;                                      /* client socket  */
    cli_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if(cli_sockfd == -1) {
      perror("Client cannot be accepted at this time, try again.");
    }
    else {                                             /* client accepted */
      int file_fd = get_file_info(cli_sockfd);
      if(file_fd != -1) {
        send_file(cli_sockfd, file_fd);
        close(file_fd);
      }
      close(cli_sockfd);
    }
  }
  close(sockfd);
  return 0;
}
