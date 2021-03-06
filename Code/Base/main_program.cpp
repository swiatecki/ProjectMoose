// Outdated

/*#include <iostream>
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
#include <queue>
#include <time.h>
#include "RobotData.cpp"
#include "logging.h"
#include "Network.h"


using namespace std;


void * recvThread(void *arg);
void *cmdThread(void *arg);
void ctrlCHandler(int s);



//Globals
 int runState = 1;



// Init a network object
 Network n;



 
int main(){
  
  // Lets catch Ctrl+C
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = ctrlCHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
  
  
  // Init Comm
  
 
  
  n.startNet();
  

  // Spawn a listner thread
  pthread_t recvThreadID;
  
  if(pthread_create(&recvThreadID,NULL,&recvThread,NULL)){
    cout << "Error in recv Thread creation" << endl;
    return -1;
  }
  
  // Spawn a command(send) thread
  pthread_t cmdThreadID;
  
  if(pthread_create(&cmdThreadID,NULL,&cmdThread,NULL)){
    cout << "Error in cmd Thread creation" << endl;
    return -1;
  }
  
  
  
  
  while(runState){
    
    // Running, do nothing
  }
  
  pthread_join(recvThreadID,NULL);
  pthread_join(cmdThreadID,NULL);
    

 
   cout << "Threads joined" << endl; 
   
  n.stopNet();
  
  
  // Write log
  
  
  
  
 return 0; 
}




void *recvThread(void *arg){
  
 vector<slogData> log;
  
  RobotData rd1(RobotData::UR5_V1x5);
  
  while(runState){
  
 // const int recvlen = 756; // package lenght. 96 values, 1 int.
  char buffer[rd1.getRecvLength()];

  int byte_count =0;

 byte_count = recv(n.getSocket(), buffer, rd1.getRecvLength(), 0);
 
 rd1.setBuffer(buffer);

 //cout << "Modtaget " << byte_count << " bytes. Data: " << ntohl(*(int*)(buffer)) << endl;


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
 rd1.getqddTarget(tmp.qddTarget); // pass array pointer to store data.
  
   //cout << "qactual tmp: " << tmp.qActual[0] << "," << tmp.qActual[1] << ","<< tmp.qActual[2] << ","<< tmp.qActual[3] << ","<< tmp.qActual[4] << "," << tmp.qActual[5] << "," << endl;
  
  log.push_back(tmp);
 
  
 
  }
  
  // runState changed..
  
  
  
  // Write logs

 writeLog(log,"test02.txt");
 
  
  

  // DONE
  
  return NULL;
  
  
  
  
}


void ctrlCHandler(int s){
  
  int signal =0;
  
  signal = s;
  
 printf("Caught signal %d \n\n\n\n\n\n\n",signal);
 
 runState = 0;
 
 
  
}



void *cmdThread(void *arg){
  
  struct timespec time1;
  
  time1.tv_sec =0;
  time1.tv_nsec = 8000000; // 8 ms / 125 hz
  
  long elapsedTicks = 0;
  long totalElapsedTicks = 0;
  
  // Sends commands to robot. Runs as thread
  
  
  
  

  // 63 = 0.5s
  cmd cmd0(250,"movel(p[-0.144, -0.530, 0.579, 2.2128, 2.0803, 0],1.2,0.3,1,0)\n"); // Go to start
  
  cmd cmd1(125,"speedl([0.1, 0, 0, 0, 0, 0],1.2,1)\n");
  cmd cmd2(125,"speedl([0.4, 0, 0, 0, 0, 0],1.2,1)\n");
  
  
  cmdList.push(cmd0);
  cmdList.push(cmd1);
  cmdList.push(cmd2);
  
  
  
  
  while(!cmdList.empty()){
    
    
    cmd currentCmd = cmdList.front();
    string cmdString = currentCmd.theCommand;
    
    
    
    
    int length = strlen(cmdString.c_str());
    
    
 
    
     write(n.getSocket(),cmdString.c_str(),length);
     
     printf("@ %ld - %s\n",totalElapsedTicks,cmdString.c_str());
     
     
     while(elapsedTicks < currentCmd.maxTicks){
       
       // Next command.
       
       nanosleep(&time1,NULL); // Lets sleep for 8ms/one tick
	elapsedTicks++;
       
    }
    
    // Its time for a new command, pop the queue
    totalElapsedTicks+=elapsedTicks;
     elapsedTicks=0;
     cmdList.pop();
  
     
    
     
  }
  

  
  

  
  
  //Ctrl handler has issued halt command. Let stop this thread
  
  return NULL;
  
  
  
}
*/




