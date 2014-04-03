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
#include "Network.h"
#include "RobotCommander.h"

using namespace std;




RobotCommander::RobotCommander(int * _socket){
  
    time1.tv_sec =0;
  time1.tv_nsec = 8000000; // 8 ms / 125 hz
  totalElapsedTicks = 0;
  elapsedTicks = 0;
  socket = _socket;
  handleDelay = true;
}


void RobotCommander::addCmd(string _command, int _maxTicks)
{
  
  
  cmd _cmd(_maxTicks,_command); // Go to start
  cmdList.push(_cmd);
  
}



int RobotCommander::run()
{
string cmdString;
int length;

   while(!cmdList.empty()){
    
    
    cmd currentCmd = cmdList.front();
     cmdString = currentCmd.theCommand;
    
    // add a linebreak \n, for correct URScript parsing
    cmdString = cmdString + "\n";
    
    
    length = strlen(cmdString.c_str());
    
     write(*socket,cmdString.c_str(),length);
     
   //  printf("@ %ld - %s\n",totalElapsedTicks,cmdString.c_str());
     
     if(handleDelay){
     while(elapsedTicks < currentCmd.maxTicks){
       
       // Next command.
       
       nanosleep(&time1,NULL); // Lets sleep for 8ms/one tick
	elapsedTicks++;
       
    }
    
    // Its time for a new command, pop the queue
    totalElapsedTicks+=elapsedTicks;
     elapsedTicks=0;
     
     }
     
     cmdList.pop();
  
     
    
     
  }
  
  
  return 0;
  
}



void RobotCommander::sethandleDelay(bool setTo)
{
handleDelay = setTo;
}



