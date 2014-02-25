#include <iostream>
#include <stdio.h>
#include <stdexcept> 

using namespace std;

class RobotData {
  

public:
  
  enum RobotType { UR5_V1x5, UR10VNxN };
  RobotData(RobotType type);
  RobotData(RobotType type, char * package);
  ~RobotData();
  
  void setBuffer(char * package);
  
  double getTime();
  void getqActual(double * returnArray);
 void getqdActual(double * returnArray);
  void getqTarget(double * returnArray);
  void getqddTarget(double *returnArray);
  
  
  
private:
  int recvlen;
  int numDoubles;
  char * buffer; 
  double * data; // 95 doubles
  char doubleTrouble[1024]; // Allocate memory for data. 

  
  struct dataMap{
    
    // Container for keeping relations between data name and positions
    int time;
    int qTarget; // Start values.
    int qactual; // Actual Joint pos.
    int qdactual; // Actual Joint velocity
    int qddTarget; // Joint traget acc. 
    
    
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
     
    // Define where the data starts
    dm.time = 0;
    dm.qTarget = 1;
    dm.qactual = 31;
    dm.qdactual = 37;
    dm.qddTarget = 13;
  

    
     
    
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
 // cout << data[1] << endl;
  
  
}

// 2nd Constructor
RobotData::RobotData(RobotType type) // constructor
{
   // Set buffer size etc. 
  
  if(type == UR5_V1x5){
    recvlen = 756;
    numDoubles = 94;
    buffer = new char[recvlen]();
   // memset(buffer,'0',recvlen); // The () in the line above replaces this, which initializes the array
    
    // Fill out datamap. 
    // OBS!! take the number from documentation and subtract 1 !! 
    // This beacuse the first value is the integer! 
     
    // Define where the data starts
    dm.time = 0;
    dm.qTarget = 1;
    dm.qactual = 31;
    dm.qdactual = 37;
    dm.qddTarget = 13;
  

    
     
    
  }else{
    // Not yet implemented.
    throw std::invalid_argument("TYPE not found");
  }
  

  
}

// Destructor

RobotData::~RobotData()
{
 // cout << "Deconstructor" <<endl;
  delete [] buffer;
}



double RobotData::getTime(){
  
  double time;
  
  time = data[dm.time];
  
  
  return time;
}

void RobotData::getqActual(double *qActual){
  
 
  
  for(int i=0;i<6;i++){
    
    qActual[i] = data[dm.qactual+i];
    
  }
  
  
}

void RobotData::getqTarget(double *qTarget){
  
 // Array size
 int size = 6;
  
  for(int i=0;i<6;i++){
    
    qTarget[i] = data[dm.qTarget+i];
    
  }
  
  
}


void RobotData::getqdActual(double *qdActual){
  
 // Array size
 int size = 6; // Returns a 6 element array..
  
  for(int i=0;i<6;i++){
    
    qdActual[i] = data[dm.qdactual+i];
    
  }
  
  
}


void RobotData::getqddTarget(double *qddTarget){
  
 // Array size
 int size = 6; // Returns a 6 element array..
  
  for(int i=0;i<6;i++){
    
    qddTarget[i] = data[dm.qddTarget+i];
    
  }
  
  
}


void RobotData::setBuffer(char * package){
  
  
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
  
  
  
}
