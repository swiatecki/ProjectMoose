#include <iostream>
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
#include "Gripper.h"

using namespace std;



  
void Gripper::startGripper()
{


  struct sockaddr_in server;
  struct in_addr ipv4addr;
  struct hostent *hp;
  
    
     sock = socket(AF_INET,SOCK_STREAM,0);
    server.sin_family = AF_INET;
    
    int i = 1;
    int opt =  setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
    
    
    struct timeval tv;

    tv.tv_sec = 2;  /* 2 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));


    inet_pton(AF_INET, HOST, &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
   

    bcopy(hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);
    server.sin_port = htons(PORT);

    connect(sock, (const sockaddr *)&server, sizeof(server));
  
    cout << "Connected to socket: " << sock << endl;
  


  
}

void Gripper::setGripper(int dist)
{

  std::ostringstream ss;  
  
  ss << "setdist "<< dist << endl;
  string s = ss.str();
  
    int length = strlen(s.c_str());
    
     write(sock,s.c_str(),length);
  
}



void Gripper::stopGripper()
{

  string stop = "stop\n";
  
  int length = strlen(stop.c_str());
    
     write(sock,stop.c_str(),length);
  
}
