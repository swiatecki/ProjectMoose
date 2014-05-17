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
#include <queue>
#include <time.h>
#include "../../Base/RobotData.cpp"
#include "logging.h"


using namespace std;

//Target host details: (ROBOT )
#define PORT 30003
#define HOST "192.38.66.249"



void * recvThread(void *arg);
void *cmdThread(void *arg);
void ctrlCHandler(int s);



//Globals
 int sd;
 int runState = 1;
 string logfilename;


 
 struct cmd{
   unsigned int maxTicks; // number of 125 hz ticks the command will live for
   string theCommand; // For storing the command
   
   cmd(int _maxTicks, string _theCommand): maxTicks(_maxTicks), theCommand(_theCommand){}
};


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
enum direction{
  accelerate,
  decelerate
  
};

direction theDir;

 
int main(int argc, char *argv[]){
  
  logfilename = "nope.txt";
  
   if(argc < 2){
    
   
cout << "Please specify either --acc or --dec" << endl;
exit(1);

    
  }else{
    for(int i=1;i<argc;++i){
      
      
      if(string(argv[i]) == "--norobot" || string(argv[i]) == "-n"){
	
	// noRobot = 1;
	  cout << " NO ROBOT " << endl;
      }
      
      if(string(argv[i]) == "--acc" || string(argv[i]) == "-a"){
	
	 theDir = accelerate;
	  cout << " Slow -> Fast (should be OK) " << endl;
	  logfilename = "accelerate.txt";
      }
      
     if(string(argv[i]) == "--dec" || string(argv[i]) == "-d"){
	
	theDir = decelerate;
	  cout << " Fast -> Slow (should fail) " << endl;
	  logfilename = "decelerate.txt";
      }
	
     
      
      if(string(argv[i]) == "--verbose" || string(argv[i]) == "-v"){
		 // verbose = 1;
	  cout << " Verbose " << endl;
      }
	
      if(string(argv[i]) == "--log"){
	logfilename = argv[i+1];
	  cout << " filename set to  " << argv[i+1] << endl;
      }
      
      
      if(string(argv[i]) == "--help" || string(argv[i]) == "-h"){
	
	  cout << "Usage: " <<endl;
	 // cout << "-n or --norobot: \t dont move robot. Good for calibrating camera. " << endl;
	//  cout << "-v or --verbose: \t Alot of nice debugging info" << endl;
	 // cout << "--log: \t Where to save the log(default: nope.txt) , ex: --log somelog.txt" << endl;
	  cout << "--acc: \t Test Accelleration - saves to accelerate.txt" << endl;
	 cout << "--dec: \t Test Deceleration - saves to decelerate.txt" << endl;
	  exit(0);
      }
	
	
	
      }
      
     
      
  }
  
  
  
  
  
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
  rd1.getqddTarget(tmp.qddTarget); // pass array pointer to store data.
  
   //cout << "qactual tmp: " << tmp.qActual[0] << "," << tmp.qActual[1] << ","<< tmp.qActual[2] << ","<< tmp.qActual[3] << ","<< tmp.qActual[4] << "," << tmp.qActual[5] << "," << endl;
  
  log.push_back(tmp);
 
 
  }
  
  // runState changed..
  
  
  
  // Write logs

 writeLog(log,logfilename);
 
  
  
  
  // DONE
  
  return NULL;
  
  
  
  
}


void ctrlCHandler(int s){
  
 printf("Caught signal %d \n\n\n\n\n\n\n",s);
 
 runState = 0;
 
 
  
}



void *cmdThread(void *arg){
  
  struct timespec time1;
  
  time1.tv_sec =0;
  time1.tv_nsec = 8000000; // 8 ms / 125 hz
  
  long elapsedTicks = 0;
  long totalElapsedTicks = 0;
  
  // Sends commands to robot. Runs as thread
  
  
  
  queue<cmd> cmdList;

  // 63 = 0.5s
//  cmd cmd0(250,"movel(p[-0.144, -0.530, 0.579, 2.2128, 2.0803, 0],1.2,0.3,1,0)\n"); // Go to start
   cmd cmd0(250,"movel(p[-0.207,  -0.546, 0.579, 1.23, -2.862, 0],1.2,0.3,1,0)\n"); // Go to start
    cmdList.push(cmd0);
    
    
  cmd cmd1(125,"speedl([0.1, 0, 0, 0, 0, 0],1.2,1)\n");
  cmd cmd2(125,"speedl([0.4, 0, 0, 0, 0, 0],1.2,1)\n");
  
   if(theDir == accelerate){
     
  cmdList.push(cmd1);
  cmdList.push(cmd2);
  
  
  }
  
  else if(theDir == decelerate){
    
      cmdList.push(cmd2);
  cmdList.push(cmd1);
  
  }
  
  
  
  
  
  


  
  
  
  
  while(!cmdList.empty()){
    
    
    cmd currentCmd = cmdList.front();
    string cmdString = currentCmd.theCommand;
    
    
    
    
    int length = strlen(cmdString.c_str());
    
    
 
    
     write(sd,cmdString.c_str(),length);
     
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





