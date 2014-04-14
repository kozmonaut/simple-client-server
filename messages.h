#include <stdlib.h>
#include <stdbool.h>

#define REQ1 0x01
#define RESP1 0x02
#define REQ2 0x03
#define RESP2 0x04

typedef struct Payload1{
    char Parameter[20];   
} Payload1;

typedef struct Payload2{
    bool Answer;
} Payload2;

struct BUFF{
    int msgId;
    int msgSize;
};
struct msgReq1{
    int msgId;
    int msgSize;
};

struct msgReq2{
    int msgId;         
    int msgSize; 
};

struct msgResp1{
   int msgId;         
   int msgSize;
   Payload1 payload1;		
};

struct msgResp2{
   int msgId;         
   int msgSize;      
   Payload2 payload2;
};
