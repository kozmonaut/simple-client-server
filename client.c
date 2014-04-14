#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <netdb.h> 
#include "messages.h"


// Error function
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    //Socket identification number
    int sockfd;
    // Message number
    int message;
    // Number of port
    int port;
    // Store address data
    struct sockaddr_in serv_addr, cli_addr;
    // Store info about host
    struct hostent *hostname;
    // Size of data structure
    socklen_t slen = sizeof(serv_addr);

    // Message initialization
     struct msgReq1 req1;
     struct msgReq2 req2;
     struct msgResp1 resp1;
     struct msgResp2 resp2;

    // Validate inserted arguments
    if (argc < 3) {
       printf("Error: Insert hostname and port.\n");
       exit(0);
    }
    // Read number of port
    port = atoi(argv[2]);

    // Create socket (UDP)
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
        error("socket");

    // Fetch hostname
    hostname = gethostbyname(argv[1]);

    // Fetch server
    if (hostname == NULL) {
        printf("Error: Unknown server\n");
        exit(0);
    }

    // Reset memory buffer
    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    //Fill structure with address, port and protocol type
    serv_addr.sin_family = AF_INET;
    bcopy((char *)hostname->h_addr, (char *)&serv_addr.sin_addr.s_addr,hostname->h_length);
    serv_addr.sin_port = htons(port);
     
    while(1){
       do{
	    printf("\nChoose request type for server(1 or 2):");
            scanf("%d",&message);
	
            if ((message> 2) || (message < 1)) {	
               printf("Error: Choose request type! \n");
            }

        // If client choose message type "1"
        if(message == 1) {
            req1.msgId = REQ1;
            req1.msgSize = sizeof(req1);
		if (sendto(sockfd,(void *) &req1, req1.msgSize, 0, (struct sockaddr*)&serv_addr, slen)==-1)
                     error("sendto()");      

		if(recvfrom(sockfd, (struct msgResp1 *) &resp1, sizeof(resp1), 0, (struct sockaddr*)&serv_addr, &slen)==-1)
                     error("recvfrom()");
          
            printf("Client %s:%d recieved response 1:\n",inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
            printf("\tParameter: %s \n", resp1.payload1.Parameter);
        }
        // If client choose message type "2"
        if(message == 2) {
            req2.msgId = REQ2;
            req2.msgSize = sizeof(req2);
	        if (sendto(sockfd,(void *) &req2, req2.msgSize, 0, (struct sockaddr*)&serv_addr, slen)==-1)
                     error("sendto()");
           
	        if(recvfrom(sockfd, (struct msgResp2 *) &resp2, sizeof(resp2), 0, (struct sockaddr*)&serv_addr, &slen)==-1)
        	     error("recvfrom()");
           
	    printf("Client %s:%d recieved response 2:\n",inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
            printf("\tAnswer: %s \n",(resp2.payload2.Answer)?"yes":"no");
       }

       }while ((message> 2) || (message < 1));
    }//End of while

    // Close socket
    close(sockfd);
    return 0;
}
