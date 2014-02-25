#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <time.h>
#include <stdint.h>
#include "../../Base/Timing.cpp"

/* TESTS THE Timing.cpp class */

using namespace std;


int main(){
   cout << "Starting" << endl;
   struct timespec time1;
  
  Timing timer0;
  
  
  timer0.setStart();
  
 
  time1.tv_sec =0;
  time1.tv_nsec = 400000000; // 0.4 s
  
  nanosleep(&time1,NULL);
  
  timer0.setStop();
  
  uint64_t etime = timer0.elapsedTimems();
  
  cout << "Elapsed time (ms): " << etime <<  endl;
  
   etime = timer0.elapsedTimeus();
   
   cout << "Elapsed time (us): " << etime <<  endl;
  
  
  return 0;
}
