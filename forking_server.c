#include "pipe_networking.h"

void sighandler(int signo){
    if(signo == SIGINT){
        printf("[forking server] WKP removed due to SIGINT.\n");
        if(remove(WKP)==-1){
            perror("[forking server] error removing WKP.\n");
            exit(1);
        }
        exit(0);

    }
}

int server_handshake_half(int *to_client, int from_client){
    int rand_num;
    rand_num = rand() %100;
    write(*to_client, &rand_num, sizeof(rand_num));
    printf("sent SYN_ACK: %d\n", rand_num);

    int ack;
    read(*to_client, &ack, sizeof(ack));
    printf("received ACK: %d\n", ack);
    
    if(ack!= rand_num +1){
        printf("incorrect ack received\n");
        exit(1);
    }
    printf("handshake complete\n");
    return *to_client;
}

int main(){
    signal(SIGINT, sighandler);
    while(1){
        // int to_client;
        int from_client = server_setup();
        int f = fork();
        if(f==0){
        int to_client = from_client;
        char client_pp[HANDSHAKE_BUFFER_SIZE];
        read(from_client, client_pp, sizeof(client_pp));
        printf("received client PP: %s\n", client_pp);
        to_client = open(client_pp, O_WRONLY);
        if(to_client == -1){
            perror("open client PP\n");
            exit(1);
        }
        printf("opened client PP\n");
        close(from_client);
        server_handshake_half(&to_client, from_client);
        int rando;
        while(1){
            rando = rand() %101;
            if(write(to_client, &rando, sizeof(int))==-1){
                perror("[forking sever] error writing.\n");
                exit(1);
            }
            printf("[forking server] sent: %d\n", rando);
            sleep(1);
        }
        exit(0);
        }
        else{
        close(from_client);
        // close(to_client);
        // printf("[persistant server] client disconnected.\n");
        }
    }
    
    return 0;
}