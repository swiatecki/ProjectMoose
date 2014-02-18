#include <iostream>
#include <stdio.h>
#include <stdexcept> 

using namespace std;

class RobotData {
  

public:
  
  enum RobotType { UR5_V1x5, UR10VNxN };
  RobotData(RobotType type, char * package);
  double getTime();
  void getqActual(double * returnArray);
  void getqTarget(double * returnArray);
  
  
private:
  int recvlen;
  int numDoubles;
  char * buffer; 
  double * data; // 95 doubles
  char doubleTrouble[1024]; // Allocate memory for data. 

  
  struct dataMap{
    
    // Container for keeping relations between data name and positions
    int time;
    int qTarget; // Start value.
    int qactual[6]; // 6;
    int qdactual[6]; // 6;
    
  }dm;

  

  

};




  //constructor
RobotData::RobotData(RobotType type, char * package) // constructor
{
   
  
  // Set buffer size etc. 
  
  if(type == UR5_V1x5){
    recvlen = 756;
    numDoubles = 94;
    buffer = new char[recvlen];
    
    // Fill out datamap. 
    // OBS!! take the number from documentation and subtract 1 !! 
    // This beacuse the first value is the integer! 
     
    dm.time = 0;
    
    
    dm.qTarget = 1;
    
    
    dm.qactual[0] = 31;
    dm.qactual[1] = 32;
    dm.qactual[2] = 33;
    dm.qactual[3] = 34;
    dm.qactual[4] = 35;
    dm.qactual[5] = 36;

    
     
    
  }else{
    // Not yet implemented.
    throw std::invalid_argument("TYPE not found");
  }
  
  buffer = package;
  
  /* DECODING */
  
  
   // Flip bits, and extract doubles
  
  //char doubleTrouble[recvlen-4]; // 4 bits for integer
  
  char * doubleBuffer;
  
  doubleBuffer = &buffer[4]; // Skip the initial double
  
 
  
    for(int i=0;i<numDoubles;i++){ // 94 doubles
    
      for(int k=0;k<8;k++){
      // For every byte in the double  
	
	doubleTrouble[(i*8)+k] = doubleBuffer[(i*8)+(7-k)];
	
      }

  }
  
  data = (double *)(&doubleTrouble);
  cout << data[1] << endl;
  
  
}

double RobotData::getTime(){
  
  double time;
  
  time = data[dm.time];
  
  
  return time;
}

void RobotData::getqActual(double *qActual){
  
 
  
  for(int i=0;i<6;i++){
    
    qActual[i] = data[dm.qactual[i]];
    
  }
  
  
}

void RobotData::getqTarget(double *qTarget){
  
 // Array size
 int size = 6;
  
  for(int i=0;i<6;i++){
    
    qTarget[i] = data[dm.qTarget+i];
    
  }
  
  
}

