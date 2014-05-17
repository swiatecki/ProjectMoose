#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <signal.h>// For catching Ctrl C
#include <queue>
#include <time.h>
#include <iomanip> // for setprecision
#include <stdlib.h>  


#include "../../Base/Gripper.h"

using namespace std;

int main(int argc, char *argv[]){
  
    int gripperDist = 30;
  
    if(argc < 2){
    cout << "did you forget -d XX?" << endl;
    
  }else{
    for(int i=1;i<argc;++i){
      
      
      if(string(argv[i]) == "--dist" || string(argv[i]) == "-d"){
	
	gripperDist = atoi(argv[i+1]);
	  cout << " Dist set to  "  << endl;
      }
	
      
      
      if(string(argv[i]) == "--help" || string(argv[i]) == "-h"){
	
	  cout << "Usage: " <<endl;
	  cout << "-h or --help: \t " << endl;

	  exit(0);
      }
	
	
	
      }
      
     
      
  }
  
  
  Gripper gr;
  
  
  gr.startGripper();
  
  
  gr.setGripper(gripperDist);
  
  gr.stopGripper();
  
  
}



