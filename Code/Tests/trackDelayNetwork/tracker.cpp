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
#include <signal.h>// For catching Ctrl C
#include <queue>
#include <time.h>
#include "../../Base/RobotData.cpp"
#include "logging.h"
#include "../../Base/Network.h"
#include "../../Base/RobotCommander.h"
#include "../../Base/Timing.cpp"


using namespace std;


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
  
  
  
  
  
  // Start 
  
  
  
 
  
 
  vector<slogData> log;
  
  RobotData rd1(RobotData::UR5_V1x5);
  
 char buffer[rd1.getRecvLength()];
  

 // Lets make a RobotCommander, for initial position
 int tmpSocket = n.getSocket();
 
 RobotCommander cmdStart(&tmpSocket);
 RobotCommander robot(&tmpSocket);
  
 
 
 // Go to start
cmdStart.addCmd("movel(p[-0.144, -0.530, 0.579, 2.2128, 2.0803, 0],1.2,0.3,1,0)",250);
cmdStart.run();
  
Timing timer0;


 
 int send =0;
  while(runState){
    
     /* READ DATA */
    
 // const int recvlen = 756; // package lenght. 96 values, 1 int.


  int byte_count =0;

 byte_count = recv(n.getSocket(), buffer, rd1.getRecvLength(), 0);
 
 if(send){
  timer0.setStop();
 }
 
 
 rd1.setBuffer(buffer);

 
 double q[6];
 rd1.getqActual(q);
 
 double qTarget[6];
 rd1.getqTarget(qTarget);
 
 cout << rd1.getTime() << endl;

//  cout << "qTarget: " << qTarget[0] << "," << qTarget[1] << ","<< qTarget[2] << ","<< qTarget[3] << ","<< qTarget[4] << "," << qTarget[5] << "," << endl;
 //  cout << "qactual: " << q[0] << "," << q[1] << ","<< q[2] << ","<< q[3] << ","<< q[4] << "," << q[5] << "," << endl;
  
 // Add to Log
 
 slogData tmp;
 
 double xx[6] = {0,0,0,0,0,0};
 
  tmp.robotTime = rd1.getTime(); // Returns double
  
 tmp.sysTime = (unsigned long)timer0.elapsedTimems();
  
  rd1.getqActual(tmp.qActual); // pass array pointer to store data.
 rd1.getqdActual(tmp.qdActual); // pass array pointer to store data.
 rd1.getqddTarget(tmp.qddTarget); // pass array pointer to store data.
 rd1.getqTarget(tmp.qTarget);
 rd1.getqdTarget(tmp.qdTarget);
  
   //cout << "qactual tmp: " << tmp.qActual[0] << "," << tmp.qActual[1] << ","<< tmp.qActual[2] << ","<< tmp.qActual[3] << ","<< tmp.qActual[4] << "," << tmp.qActual[5] << "," << endl;
  
  log.push_back(tmp);
 
   
  /* 
   * 
   * Controller
   * 
   * */
  
  
  
  /* 
   * 
   * WRITE DATA
   * 
   * */
  
  if(!send){
  robot.addCmd("speedl([0.4, 0, 0, 0, 0, 0],0.8,1)",125);
  timer0.setStart();
  robot.run();
  send=1;
 
  }

  
 
  }
  
  // runState changed..
  
  
  
  // Write logs


  
  
 /* 
  * 
  * End read data.
  */
  

  
  
    writeLog(log,"delayLog01.txt");
 
   cout << "Exiting" << endl; 
   
  n.stopNet();
  
  
  // Write log
  
  
  
  
 return 0; 
}






void ctrlCHandler(int s){
  
  int signal =0;
  
  signal = s;
  
 printf("Caught signal %d \n\n\n\n\n\n\n",signal);
 
 runState = 0;
 
 
  
}

void goToStart(){
  
  
  
  
  
  
  
}





