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
#include <iomanip> // for setprecision
#include <pthread.h>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../../Base/RobotData.cpp"
#include "logging.h"
#include "../../Base/Network.h"
#include "../../Base/RobotCommander.h"
#include "../../Base/Timing.cpp"


using namespace std;

int debugMonitor =0;
int noRobot = 0;
int noCamera = 0;
int verbose =0;

void ctrlCHandler(int s);


void *cameraThread(void *arg);
void *robotThread(void *arg);


void shutterCB(int pos, void* param);

//Globals
 int runState = 1;

  cv::VideoCapture cap;  



// Init a network object
 Network n;

  string logfilename;
  
 RobotData rd1(RobotData::UR5_V1x5);
 
 
 
 
struct cmdData{

double x,y,z,r,p,ya,a,t_min,distX,distY;

};


struct coord{

double x,y;

}cameraError;

//
 
  
 
int main(int argc, char *argv[]){

  
 
  
  if(argc < 2){
    
    // Default
    logfilename = "delayLogDefault.txt";


    
  }else{
    for(int i=1;i<argc;++i){
      
      
      if(string(argv[i]) == "--norobot"){
	
	 noRobot = 1;
	  cout << " NO ROBOT " << endl;
      }
	
      if(string(argv[i]) == "--debugMonitor"){
		  debugMonitor = 1;
	  cout << " Debug enabled " << endl;
      }
      
      if(string(argv[i]) == "--verbose"){
		  verbose = 1;
	  cout << " Verbose " << endl;
      }
	
      if(string(argv[i]) == "--log"){
	logfilename = argv[i+1];
	  cout << " filename set to  " << argv[i+1] << endl;
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


n.startNet();


// Spawn a cameraThread
  
  pthread_t cameraThreadID;
  
  if(pthread_create(&cameraThreadID,NULL,&cameraThread,NULL)){
    cout << "Error in recv Thread creation" << endl;
    return -1;
  }

  
  // Spawn a robotThread
  
  pthread_t robotThreadID;
  
  if(pthread_create(&robotThreadID,NULL,&robotThread,NULL)){
    cout << "Error in recv Thread creation" << endl;
    return -1;
  }
  
  
  
  
  ///



 

 
 /*
  * 
  * WHILE
  */
 
 
  while(runState){
    
   
  }
  
  // runState changed..
  
  
  
  // Write logs


  
  
 /* 
  * 
  * End read data.
  */
  

 pthread_join(cameraThreadID,NULL);
 pthread_join(robotThreadID,NULL);
    
 
   cout << "Threads joined" << endl; 
  
  
 

  n.stopNet();
  cout << "network closed" << endl; 
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

void *cameraThread(void *arg){
  
 
std::vector<cmdData> cmd;
  cv::Mat frame; 
  
  cv::namedWindow("Color", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"

cv::namedWindow("Thresholded", CV_WINDOW_AUTOSIZE); //create a window with the name "HSV"
cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
int initShutter = 106;
//int initShutter = 0;

int shutterVal = initShutter;
int cannyMin = 125;


int fps = 60;


// Shutter slider
cv::createTrackbar("Shutter","Color",&shutterVal,4095,shutterCB,NULL);

// Canny treshold

cv::createTrackbar("Threshold","Color",&cannyMin,255,NULL,NULL);

cv::Mat colorFrame;
cv::Mat tresholdedFrame;
cv::Mat hsvFrame;
cv::Mat grey,tmp;
cv::Mat contourOutput;

  


cap.open(CV_CAP_DC1394); // Open first firewire camera. in 2.3 use CV_CAP, in 2.5 use CV::CAP


cap.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U,794); // 736
cap.set(CV_CAP_PROP_WHITE_BALANCE_RED_V,437);
cap.set(CV_CAP_PROP_EXPOSURE,initShutter); // "Shutter" in coriander
cap.set(CV_CAP_PROP_FPS,fps);
cap.set(CV_CAP_PROP_GAMMA,0);
cap.set(CV_CAP_PROP_GAIN,30);

while(runState){

 
cap >> frame;


std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> hierarchy;


// Get color image, decode bayer BGGR.  
cv::cvtColor(frame,colorFrame,CV_BayerBG2RGB,0);
cv::cvtColor(colorFrame, grey, CV_RGB2GRAY );


// Remove gripper from img
cv::Rect roi = cv::Rect(0,0,640,360);
cv::Mat submatrix = cv::Mat(grey,roi);
//submatrix.setTo(cv::Scalar(255));

cv::threshold(submatrix,tresholdedFrame,cannyMin,255,cv::THRESH_BINARY_INV);




cv::Canny( tresholdedFrame, tmp, cannyMin, cannyMin*2, 3 );


cv::findContours(tmp,contours,hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

int blablalba = 0 ;

 /// Get the moments
  std::vector<cv::Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  ///  Get the mass centers:
  std::vector<cv::Point2f> mc( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

 
    std::vector<cv::Rect> boundRect( contours.size() );
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
// Draw some countours, and maybe some bounding box
 
 
cv::Mat drawing = cv::Mat::zeros( tresholdedFrame.size(), CV_8UC3 );


cv::Point centerOfBlock;

  for( int i = 0; i< contours.size(); i++ )
     {

	// Filter out small areas, and filter out contours that are holes 
	// See http://stackoverflow.com/questions/8461612/using-hierarchy-in-findcontours-in-opencv
       if(cv::contourArea(contours[i]) < 200  ){
	
	 
      }else{
     
       
       cv::Scalar color = cv::Scalar( 0,255,0 );
	//draw center of mass
      cv::circle( drawing, mc[i], 5, color, -1, 8, 0 );
	
	
	cv::Point xx = cv::Point(boundRect[i].tl().x+(boundRect[i].width/2),boundRect[i].tl().y+(boundRect[i].height/2));
	//cv::circle( drawing, xx, 2, color, -1, 8, 0 );
	 
	centerOfBlock = mc[i];
	
	
      }
     }
     
     

  /*
   * Calculate distance. 
   
   */
  
  
     
   cv::Size s = tresholdedFrame.size(); 
     
   cv::Point centerOfFrame = cv::Point(s.width/2,s.height/2);  
     
   float distX = centerOfFrame.x-centerOfBlock.x;
   
   float distY = centerOfFrame.y-centerOfBlock.y;
   
   if(centerOfBlock.x == 0 || centerOfBlock.y == 0 || (fabs(distX) < 3 && fabs(distY) < 3)){
	    
  

     
  }else{
    

   // cout << "(Dist X, Dist Y) " << distX << ", " << distY << endl;
  
    cameraError.x = distX;
    cameraError.y = distY;
    
  //  std::cout << outss << std::endl;
/*cmdData tmp;

tmp.x = x_out;
tmp.y = y_out;
tmp.z = 0;
tmp.r = 0;
tmp.p =0; 
tmp.ya =0;
tmp.a = a_out;
tmp.t_min = t_min_out;
tmp.distX = distX;
tmp.distY = distY;

cmd.push_back(tmp);
*/
    
    
  }

   
  

   
     

 


if(!frame .data) break;


if(debugMonitor){
  
  cv::imshow("Thresholded",tresholdedFrame); // Uncomment this line to see the actual picture. It will give an unsteady FPS


cv::imshow("Color",grey); // Uncomment this line to see the actual picture. It will give an unsteady FPS

cv::imshow( "Contours", drawing );
}




if(cv::waitKey(1) >= 27){ break;  } // We wait 1ms - so that the frame can be drawn. break on ESC



}

cv::destroyWindow("Color"); //destroy the window with the name, "MyWindow"
cv::destroyWindow("Thresholded"); 



}

// ROBOT !! 


void* robotThread(void* arg)
{
 
  Timing timer0;
  
char buffer[rd1.getRecvLength()];
 double q[6];
 double qTarget[6];
 
 double nonLimitedSignal;

int counter = 0;

 timer0.setStart();
 
// double ref = 
 // double ref = 1.6;
 
 double e,signal = 0.0;
 double initE =0;
 double initBase=0;  
 
 vector<slogData> log;
 
  int socket = n.getSocket();
  RobotCommander robot(&socket);
  
  bool off = false;
  robot.sethandleDelay(off);

  Timing sleeper;
  sleeper.setStart();
  
  
  
  
  
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
  
  
  e = cameraError.x;
  
  if(initE == 0){
    initE = e;
    initBase = tmp.qActual[0];
  }
  
  
 
   //signal = e*18.1970;
  signal = e/400; // 450 er stabil
  
  nonLimitedSignal = signal;
  
   if(signal> 3.2){
     signal = 3.2;
  }else if(signal < -3.2){
    
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
  
  
  

    
  // override all! 
  
  signal = 0;
      
std::ostringstream strs;
strs << "speedj([" << signal <<  ", 0, 0, 0, 0, 0],15,1)";
std::string cmd = strs.str();
    
if(verbose){
  
cout << cmd << endl;
cout << setprecision(9) << "Non limited output : " << nonLimitedSignal << ", Limited: " << signal << endl;
  
}

      
  if(!noRobot){
    
    sleeper.setStop();
    
    if(sleeper.elapsedTimeus() >= 16000){ // 20000 is OK, 16700 is OK, 
      
          
    robot.addCmd(cmd,0);
    robot.run();
       
 sleeper.setStart();
      
    }

  
  }
  
  
   struct timespec time1;
  time1.tv_sec =0;
  time1.tv_nsec = 7000000; // 8 ms / 125 hz
      
   nanosleep(&time1,NULL); // Lets sleep for 8ms/one tick
    
 


  
  
  
  counter++;
  
   
  }
  
  
     writeLog(log,logfilename);
  
   
   cout << "Exiting" << endl; 
    cout << "InitE = " << initE << endl; 
    cout << "InitBase = " << initBase << endl;
  
  
}




  
void shutterCB(int pos, void* param){
struct timeval t;

cap.set(CV_CAP_PROP_EXPOSURE,pos);

//fcount=0; // Reset frame counter, so we dont have do wait for the avg to "catch" up



std::cout << "CALLBACK !!!: pos:  " << pos << "Shutter read: " << cap.get(CV_CAP_PROP_EXPOSURE) << std::endl;
}

  



