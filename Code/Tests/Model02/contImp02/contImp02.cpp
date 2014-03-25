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
#include "../../../Base/RobotData.cpp"
#include "logging.h"
#include "../../../Base/Network.h"
#include "../../../Base/RobotCommander.h"
#include "../../../Base/Timing.cpp"


using namespace std;


void ctrlCHandler(int s);



//Globals
 int runState = 1;





// Init a network object
 Network n;



 
int main(int argc, char *argv[]){
  string logfilename;
  
  
  if(argc < 2){
    logfilename = "delayLogDefault.txt";
    
  }else{
      logfilename = argv[1];
  }

  
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
cmdStart.addCmd("movel(p[-0.144, -0.530, 0.579, 2.2128, 2.0803, 0],1.0,0.3,0,0)",250);
cmdStart.run();
  

n.stopNet();
// End goto intial posistion



n.startNet();

//

Timing timer0;


 
 double q[6];
 double qTarget[6];
 
 

int counter = 0;

 timer0.setStart();
 
 double ref = -1.2;
 // double ref = 1.6;
 
 double e,signal = 0.0;
 double initE =0;
 double initBase=0;
 
 /*
  * 
  * WHILE
  */
 
 
  while(runState){
    
     /* READ DATA */
    
 // const int recvlen = 756; // package lenght. 96 values, 1 int.


  int byte_count =0;
// MSG_WAITALL sets blocking
  

  
 byte_count = recv(n.getSocket(), buffer, rd1.getRecvLength(), MSG_WAITALL);
 
 if(byte_count < rd1.getRecvLength()){
   
   //something went wrong
   
   cout << "CRITIAL ERROR!!! - Network fail. Bytes recv: " << byte_count << endl;
   break;
}
 
 

  timer0.setStop();
 
 //timer0.setStop();
 
 rd1.setBuffer(buffer);

 

 rd1.getqActual(q);
 rd1.getqTarget(qTarget);
 
 //cout << rd1.getTime() << endl;

//  cout << "qTarget: " << qTarget[0] << "," << qTarget[1] << ","<< qTarget[2] << ","<< qTarget[3] << ","<< qTarget[4] << "," << qTarget[5] << "," << endl;
 //  cout << "qactual: " << q[0] << "," << q[1] << ","<< q[2] << ","<< q[3] << ","<< q[4] << "," << q[5] << "," << endl;
  
 // Add to Log
 
 slogData tmp;
 
 
  tmp.robotTime = rd1.getTime(); // Returns double
  
 tmp.sysTime = timer0.elapsedTimeus();
  
 
  rd1.getqActual(tmp.qActual); // pass array pointer to store data.
 rd1.getqdActual(tmp.qdActual); // pass array pointer to store data.
 rd1.getqddTarget(tmp.qddTarget); // pass array pointer to store data.
 rd1.getqTarget(tmp.qTarget);
 rd1.getqdTarget(tmp.qdTarget);
 tmp.test01 = 0;
  
   //cout << "qactual tmp: " << tmp.qActual[0] << "," << tmp.qActual[1] << ","<< tmp.qActual[2] << ","<< tmp.qActual[3] << ","<< tmp.qActual[4] << "," << tmp.qActual[5] << "," << endl;
  
  log.push_back(tmp);

    
 
  /* 
   * 
   * Controller
   * 
   * */
  
  
  e = ref-tmp.qActual[0];
  
  if(initE == 0){
    initE = e;
    initBase = tmp.qActual[0];
  }
  
  
 
   signal = e*18.1970;
  //signal = e*16;
  
   if(signal> 3.2){
     signal = 3.2;
  }else if(signal< -3.2){
    
    signal = -3.2;
  }
  
  
  
  
  if(signal < 0.01 && signal > 0 ){
    signal = 0;
  }else if(signal > -0.01 && signal < 0){
        signal = 0;
  }
  
  
  
  /* 
   * 
   * WRITE DATA
   * 
   * */
  
  
  

    
    
      
std::ostringstream strs;
strs << "speedj([" << signal <<  ", 0, 0, 0, 0, 0],15,1)";
std::string cmd = strs.str();
    
      
  robot.addCmd(cmd,0);
  robot.run();

   struct timespec time1;
  time1.tv_sec =0;
  time1.tv_nsec = 8000000; // 8 ms / 125 hz
      
   nanosleep(&time1,NULL); // Lets sleep for 8ms/one tick
    
 


  
  
  
  counter++;
  
 
  }
  
  // runState changed..
  
  
  
  // Write logs


  
  
 /* 
  * 
  * End read data.
  */
  

  
  
    writeLog(log,logfilename);
 
   cout << "Exiting" << endl; 
    cout << "InitE = " << initE << endl; 
    cout << "InitBase = " << initBase << endl;
  n.stopNet();
  
  // Write log
  
  
  
  
 return 0; 
}






void ctrlCHandler(int s){
  
  int signal =0;
  
  signal = s;
  
// printf("Caught signal %d \n\n\n\n\n\n\n",signal);
 
 runState = 0;
 
 
  
}

void goToStart(){
  
  
  
  
  
  
  
}





