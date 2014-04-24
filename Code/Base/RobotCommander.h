#ifndef ROBOTCOMMANDER_H
#define ROBOTCOMMANDER_H

#include <queue>

class RobotCommander{
  
 
  
public:
  RobotCommander(int * _socket);
  RobotCommander();
  void addCmd(std::string _command, int _maxTicks);
  int run();
  
   void sethandleDelay(bool setTo);
   void setSocket(int * _socket);
    
   struct cmd{
   unsigned int maxTicks; // number of 125 hz ticks the command will live for
   std::string theCommand; // For storing the command
   
   cmd(int _maxTicks, std::string _theCommand): maxTicks(_maxTicks), theCommand(_theCommand){}
   
   

};

  
  
  
  
private:
  std::queue<cmd> cmdList;
  struct timespec time1;
  
  
  long elapsedTicks;
  long totalElapsedTicks;
  int * socket;
  bool handleDelay;
};

#endif