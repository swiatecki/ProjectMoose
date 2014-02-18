#include <iostream>
#include <algorithm>
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
#include <pthread.h>
#include <signal.h>// For catching Ctrl C
#include "RobotData.cpp"
#include "logging.h"

using namespace std;

//Target host details: (ROBOT )
#define PORT 30003
#define HOST "192.38.66.249"



void * recvThread(void *arg);
void ctrlCHandler(int s);



//Globals
 int sd;
 int runState = 1;




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




void stopNet(){
  
  close(sd);
  
}

 
int main(){
  
  // Lets catch Ctrl+C
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = ctrlCHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
  
  
  // Init Comm
  
  startNet();

  // Spawn a listner thread
  pthread_t recvThreadID;
  
  if(pthread_create(&recvThreadID,NULL,&recvThread,NULL)){
    cout << "Error in Thread creation" << endl;
    return -1;
  }
  
  
  
  while(runState){
    
    // Running, do nothing
  }
  
  pthread_join(recvThreadID,NULL);
  cout << "Threads joined" << endl; 
  
  
  stopNet();
  
  
  // Write log
  
  
  
  
 return 0; 
}




void *recvThread(void *arg){
  
 vector<slogData> log;
  
 
  
  while(runState){
   const int recvlen = 756; // package lenght. 96 values, 1 int.
  char buffer[recvlen];

  int byte_count =0;

 byte_count = recv(sd, buffer, recvlen, 0);
 
 

 //cout << "Modtaget " << byte_count << " bytes. Data: " << ntohl(*(int*)(buffer)) << endl;

 RobotData rd1(RobotData::UR5_V1x5,buffer);
 //cout << "Time: " << rd1.getTime() << endl;

 
 double q[6];
 rd1.getqActual(q);
 
 double qTarget[6];
 rd1.getqTarget(qTarget);
 
 
 
 
 
//  cout << "qTarget: " << qTarget[0] << "," << qTarget[1] << ","<< qTarget[2] << ","<< qTarget[3] << ","<< qTarget[4] << "," << qTarget[5] << "," << endl;
 //  cout << "qactual: " << q[0] << "," << q[1] << ","<< q[2] << ","<< q[3] << ","<< q[4] << "," << q[5] << "," << endl;
  
 // Add to Log
 
 slogData tmp;
 
 double xx[6] = {0,0,0,0,0,0};
 
  tmp.robotTime = rd1.getTime(); // Returns double
  
  tmp.sysTime =0;
  
  rd1.getqActual(tmp.qActual); // pass array pointer to store data.
  rd1.getqdActual(tmp.qdActual); // pass array pointer to store data.
  
  
   //cout << "qactual tmp: " << tmp.qActual[0] << "," << tmp.qActual[1] << ","<< tmp.qActual[2] << ","<< tmp.qActual[3] << ","<< tmp.qActual[4] << "," << tmp.qActual[5] << "," << endl;
  
  log.push_back(tmp);
 
 
  }
  
  // runState changed..
  
  
  
  // Write logs

 writeLog(log,"test01.txt");
 
  
  
  
  // DONE
  
  return NULL;
  
  
  
  
}


void ctrlCHandler(int s){
  
 printf("Caught signal %d \n\n\n\n\n\n\n",s);
 
 runState = 0;
 
 
  
}

