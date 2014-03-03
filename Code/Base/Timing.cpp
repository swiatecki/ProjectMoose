#include <iostream>
#include <stdio.h>
#include <stdexcept> 
#include <sys/time.h>
#include <ctime>
#include <stdint.h>

using namespace std;

class Timing {

  // Inspired by http://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c
  
private:
  
  struct timeval start, stop;
  uint64_t test; 
  
public:
  void setStart();
  void setStart(uint64_t time); // nIMP
  void setStop();
  long elapsedTimeSeconds();
  uint64_t elapsedTimems();
  uint64_t elapsedTimeus();


};



void Timing::setStart(){

  /* Set start time to NOW */
  
  gettimeofday(&start, NULL);
  
  
}

void Timing::setStart(uint64_t time){

  //Set start time to the parameter 
  
  
 // NOT YET IMPLEMENTED 
  
}

void Timing::setStop(){

  /* Set start time to NOW */
  
  gettimeofday(&stop, NULL);
  
}

uint64_t Timing::elapsedTimeus(){
  
  /* Returns elapsed time in us (micro seconds) */
  
  uint64_t startus = start.tv_usec;
  uint64_t stopus = stop.tv_usec;
  uint64_t elapsed= 0;
  
  // Add the seconds after converting to us;
  startus += (start.tv_sec*1000000);
  stopus += (stop.tv_sec*1000000);
  
  elapsed = stopus - startus;
  
  
  return elapsed;
 
}

uint64_t Timing::elapsedTimems(){
  
  /* Returns elapsed time in ms */
  
  uint64_t elapsedTimems = 0;
  
  
  // First get the elapsed time in micro seconds (us)
  uint64_t elapsedus = elapsedTimeus();
  

  
  // Convert from us to ms 
 
  elapsedTimems = (uint64_t) elapsedus/1000;
  
  
  //  cout << elapsedTimems << endl;
    
  return elapsedTimems;
  
  
}

