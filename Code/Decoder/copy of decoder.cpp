#include <iostream>
#include <algorithm>
using namespace std;

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <sstream>


//Target host details: (ROBOT )
#define PORT 30003
#define HOST "192.38.66.249"

 int sd;
 




void startNet(){
  
  struct sockaddr_in server;
  struct in_addr ipv4addr;
  struct hostent *hp;
  
    
     sd = socket(AF_INET,SOCK_STREAM,0);
    server.sin_family = AF_INET;
    
    int i = 1;
    int opt =  setsockopt( sd, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
    
    cout << opt << endl;

    inet_pton(AF_INET, HOST, &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
   

    bcopy(hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);
    server.sin_port = htons(PORT);

    connect(sd, (const sockaddr *)&server, sizeof(server));
  
    cout << sd << endl;
  
}


void readSocket(){
  const int recvlen = 756; // package lenght. 96 values, 1 int.
  char buffer[recvlen];
  char doubleBuffer[recvlen-4];
  char doubleTrouble[recvlen-4];
 
 
 
  int byte_count =0;
  
  double * data; // 95 doubles

 byte_count = recv(sd, buffer, recvlen, 0);
 
 
memcpy (doubleBuffer, &buffer[4], recvlen-4) ;

 cout << "Modtaget " << byte_count << " bytes. Data: " << ntohl(*(int*)(buffer)) << endl;

for(int i=0;i<94;i++){ // 94 doubles
  
  for(int k=0;k<8;k++){
  // For every byte in the double  
    
    doubleTrouble[(i*8)+k] = doubleBuffer[(i*8)+(7-k)];
    
  }

}
 
 
     data = (double *)(&doubleTrouble);
     cout << (data[73]) << endl;

  
  
  
  
}


void stopNet(){
  
  close(sd);
  
}

 
int main(){
  
  startNet();
  readSocket();
  stopNet();
  
  
  
 return 0; 
}
