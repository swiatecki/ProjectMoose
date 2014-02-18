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

#include "RobotData.cpp"


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

  int byte_count =0;

 byte_count = recv(sd, buffer, recvlen, 0);
 
 

 cout << "Modtaget " << byte_count << " bytes. Data: " << ntohl(*(int*)(buffer)) << endl;

 RobotData rd1(RobotData::UR5_V1x5,buffer);
 cout << "Time: " << rd1.getTime() << endl;

 
 double q[6];
 rd1.getqActual(q);
 
 double qTarget[6];
 rd1.getqTarget(qTarget);
 
  cout << "qTarget: " << qTarget[0] << "," << qTarget[1] << ","<< qTarget[2] << ","<< qTarget[3] << ","<< qTarget[4] << "," << qTarget[5] << "," << endl;
   cout << "qactual: " << q[0] << "," << q[1] << ","<< q[2] << ","<< q[3] << ","<< q[4] << "," << q[5] << "," << endl;
  
  
  
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
