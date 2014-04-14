#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <errno.h>
#include "messages.h"

// Error function
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     //Socket identification number
     int sockfd;
     // Number of port
     int port;
     // Store address data
     struct sockaddr_in serv_addr, cli_addr;
     // Size of data structure
     socklen_t clen = sizeof(cli_addr); 
   
     // Message initialization
     struct BUFF buffer;
     struct msgReq1 req1;
     struct msgReq2 req2;
     struct msgResp1 resp1;
     struct msgResp2 resp2;

     char parameter[20]="some_parameter";		
     char string;

     // Validate inserted arguments
     if (argc < 2) {
         printf("Error: Type port number.\n");
         exit(1);
     }
     // Create socket (UDP)
     if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
      error("Error: Socket not created!\n");
       	else
      printf("Server: Socket succesfully created.\n");

     // Reset memory buffer
     memset((char *) &serv_addr, 0, sizeof(serv_addr));
     // Read number of port
     port = atoi(argv[1]);
     // Use AF_INET from protocol family
     serv_addr.sin_family = AF_INET;
     // Use any interface
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     // Convert port number to network byte order
     serv_addr.sin_port = htons(port);

     // Connect socket with port
     if (bind(sockfd, (struct sockaddr* ) &serv_addr, sizeof(serv_addr))==-1)
     	    error("Server: Socket unsuccesfully connected with port.");
        else
            printf("Server: Socket succesfully connected with port %d. Waiting for data... \n",port);
            printf("==============\n");

     // Listen connections
     listen(sockfd, 1);

     // Waiting for client message
     while(1) {

        // Server recieve data from client
        recvfrom(sockfd, (struct BUFF *) &buffer, sizeof(buffer), 0, (struct sockaddr*)&cli_addr, &clen);
	
        // If server recieve request "1"
        if(buffer.msgId == REQ1) {
            resp1.msgId = RESP1;
            resp1.msgSize = sizeof(resp1);
            parameter[20] = string; 
            strcpy(resp1.payload1.Parameter, parameter);

                  sendto(sockfd, (void *) &resp1, resp1.msgSize, 0, (struct sockaddr*)&cli_addr, clen);

            printf("\nServer %s:%d sent response 1:\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            printf("\tParameter: %s \n", resp1.payload1.Parameter);
        }
        // If server recieve request "2"
        if(buffer.msgId == REQ2) {
            resp2.msgId = RESP2;
            resp2.msgSize = sizeof(resp2);
            resp2.payload2.Answer = !resp2.payload2.Answer;

                   sendto(sockfd, (void *) &resp2, resp2.msgSize, 0, (struct sockaddr*)&cli_addr, clen);
   
            printf("\nServer %s:%d sent response 2:\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            printf("\tAnswer:%s\n",(resp2.payload2.Answer)?"yes":"no");
        } 
    }
  
    // Close socket
    close(sockfd);

    return 0;
}
