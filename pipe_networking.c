#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;
  return from_client;
}

/*=========================
  server_handshake 
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client;
  printf("[server] creating wkp...\n");
  if(mkfifo(WKP, 0644) == -1){
    perror("[server] error creating wkp");
    exit(1);
  }

  printf("[server] waiting for a connection");
  from_client = open(WKP, O_RDONLY);
  if(from_client == -1){
    perror("[server] error opening wkp");
    exit(1);
  }

  char client_pipe[HANDSHAKE_BUFFER_SIZE];
  if(read(from_client, client_pipe, sizeof(client_pipe)) == -1){
    perror("[server] error reading from wkp");
    exit(1);
  }
  printf("[server] received client pipe name: %s\n", client_pipe);
  if(remove(WKP) == -1){
    perror("[server] error removing wkp");
    exit(1);
  }
  printf("[server] WKP removed.\n");
  *to_client = open(client_pipe, O_WRONLY);
  if(*to_client == -1){
    perror("[server] error opening client pipe");
    exit(1);
  }

  const char *acknowledgment = "ACK";
  if(write(*to_client, acknowledgment, strlen(acknowledgment) +1)==-1){
    perror("[server] error writing to client pipe");
    exit(1);
  }
  printf("[server] sent acknowledgment to client.\n");
  
  char buffer[HANDSHAKE_BUFFER_SIZE];
  if(read(from_client, buffer, sizeof(buffer)) == -1){
    perror("[server] error reading client acknowledgment");
    exit(1);
  }
  printf("[server] received acknowledgment from client: %s\n", buffer);
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;
  char private_pipe[HANDSHAKE_BUFFER_SIZE];
  sprintf(private_pipe, "pipe_%d", getpid());
  printf("[client] creating private pipe: %s\n", private_pipe);

  if(mkfifo(private_pipe, 0644) == -1){
    perror("[client] error creating private pipe");
    exit(1);
  }
  *to_server = open(WKP, O_WRONLY);
  if(*to_server == -1){
    perror("[client] error opening WKP");
    exit(1);
  }

  if(write(*to_server, private_pipe, strlen(private_pipe) +1) == -1){
    perror("[client] error writing to WKP");
    exit(1);
  }
  printf("[client] sent private pipe name to server.\n");

  from_server = open(private_pipe, O_RDONLY);
  if(from_server == -1){
    perror("[client] error opening private pipe");
    exit(1);
  }

  char buffer[HANDSHAKE_BUFFER_SIZE];
  if(read(from_server, buffer, sizeof(buffer)) == -1){
    perror("[client] error reading from private pipe");
    exit(1);
  }

  printf("[client] received acknowledgment from server: %s\n", buffer);

  const char *acknowledgment = "ACK";
  if(write(*to_server, acknowledgment, strlen(acknowledgment)+1) == -1){
    perror("[client] error writing acknowledgment to server");
    exit(1);
  }
  printf("[client] sent acknowledgment to server.\n");

  if(remove(private_pipe) == -1){
    perror("[client] error removing private pipe");
    exit(1);
  }
  printf("[client] private pipe removed.\n");
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  
  int to_client  = 0;
  return to_client;
}


