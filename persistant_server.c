#include "pipe_networking.h"

void sighandler(int signo){
    if(signo == SIGINT){
        printf("[perisistant server] WKP removed due to SIGINT.\n");
        if(remove(WKP)==-1){
            perror("[persistant server] error removing WKP.\n");
            exit(1);
        }
        exit(0);

    }
}

int main(){
    signal(SIGINT, sighandler);
    while(1){
        int to_client;
        int from_client = server_setup();
        to_client = from_client;
        from_client = server_handshake(&to_client);
        int rando;
        while(1){
            rando = rand() %101;
            if(write(to_client, &rando, sizeof(int))==-1){
                perror("[persistant server] error writing.\n");
                exit(1);
            }
            printf("[persistant server] sent: %d\n", rando);
            sleep(1);
        }
        close(from_client);
        close(to_client);
        printf("[persistant server] client disconnected.\n");
    }
    return 0;
}