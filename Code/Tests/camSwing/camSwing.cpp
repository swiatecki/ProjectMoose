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
#include "../../Base/Guppy.h"

#define _USE_MATH_DEFINES

using namespace std;

int debugMonitor =0;
int noRobot = 0;
int noCamera = 0;
int verbose =0;
int saveImgs =0;
int histogram =0;
int controller =0;
double gain = 10;

std::map<int,string> errMsg;



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
 
 RobotCommander robot;
 slogData tmp;
 vector<slogData> robotLog;
 
bool securityStop = false;
string reason;  
 
 
struct cmdData{

double x,y,z,r,p,ya,a,t_min,distX,distY;

};


struct coord{

int x,y;
int areaOfObject;
int ready;
}cameraError;

//
 


string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}







 
int main(int argc, char *argv[]){

  errMsg[0] = "AoO to large";
  errMsg[1] = "Out of operation area";
 
   // Default
    logfilename = "defaultLog.txt";
    
    
  if(argc < 2){
    
   


    
  }else{
    for(int i=1;i<argc;++i){
      
      
      if(string(argv[i]) == "--norobot" || string(argv[i]) == "-n"){
	
	 noRobot = 1;
	  cout << " NO ROBOT " << endl;
      }
	
      if(string(argv[i]) == "--debugMonitor" || string(argv[i]) == "--debugmonitor" || string(argv[i]) == "-d"){
		  debugMonitor = 1;
	  cout << " Debug enabled " << endl;
      }
      
        if(string(argv[i]) == "--saveImgs" || string(argv[i]) == "-s"){
		  saveImgs = 1;
	  cout << " Saving images " << endl;
      }
      
      if(string(argv[i]) == "--histogram"){
		  histogram = 1;
	  cout << " Showing histogram " << endl;
      }
      
      if(string(argv[i]) == "--verbose" || string(argv[i]) == "-v"){
		  verbose = 1;
	  cout << " Verbose " << endl;
      }
	
      if(string(argv[i]) == "--log"){
	logfilename = argv[i+1];
	  cout << " filename set to  " << argv[i+1] << endl;
      }
      
       if(string(argv[i]) == "--controller "|| string(argv[i]) == "-c") {
	controller = 1;
	  cout << " Controller active. The loop has been closed" << endl;
      }
      
      if(string(argv[i]) == "--help" || string(argv[i]) == "-h"){
	
	  cout << "Usage: " <<endl;
	  cout << "-n or --norobot: \t dont move robot. Good for calibrating camera. " << endl;
	  cout << "-d or --debugmonitor: \t Shows camera/vision output. May slow down robot " << endl;
	  cout << "-s or --saveImgs: \t Save camera imgs to imgs/" << endl;
	  cout << "-v or --verbose: \t Alot of nice debugging info" << endl;
	  cout << "-c or --controller: \t Enable controller. Close the loop." << endl;
	  cout << "--log: \t Where to save the log(default: defaultLog.txt) , ex: --log somelog.txt" << endl;
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
 
  n.startNet();
  
  
 
  

 // Lets make a RobotCommander, for initial position
 int tmpSocket = n.getSocket();
 char buffer[rd1.getRecvLength()];
 RobotCommander cmdStart(&tmpSocket);

  
 
 
 // Go to start
cmdStart.addCmd("movel(p[0.193,  -0.546, 0.643, 1.23, -2.862, 0],1.0,0.3,0,0)",250);
cmdStart.run();
  

n.stopNet();
// End goto intial posistion



n.startNet();


//RobotCommander that is shared between threads

  int socket = n.getSocket();
  robot.setSocket(&socket);

  bool off = false;
  robot.sethandleDelay(off);

//

cameraError.ready =0;
cameraError.x = -999;
cameraError.y = -999;
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
  

  Timing tmr0;
  Timing tmrFrame,tmrProcessing,tmrIdle;
  vector<cameraLog> camLog;
  
std::vector<cmdData> cmd;
  cv::Mat frame; 
  
  cv::namedWindow("Color", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"

cv::namedWindow("Thresholded", CV_WINDOW_AUTOSIZE); //create a window with the name "HSV"


cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE); //create a window with the name "HSV"

//cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
// int initShutter = 952;
int initShutter = 174; // max 800 for 60 fps

int shutterVal = initShutter;
int cannyMin = 61;
//int cannyMin = 43;



int blockSize = 89;

int fps = 60;


// Shutter slider
cv::createTrackbar("Shutter","Color",&shutterVal,4095,shutterCB,NULL);

// Canny treshold

cv::createTrackbar("Threshold","Color",&cannyMin,255,NULL,NULL);
cv::createTrackbar("BlockSize","Color",&blockSize,255,NULL,NULL);

cv::Mat colorFrame;
cv::Mat tresholdedFrame;
cv::Mat hsvFrame;
cv::Mat grey;
cv::Mat contourOutput;
cv::Mat histo;

  


cap.open(CV_CAP_DC1394); // Open first firewire camera. in 2.3 use CV_CAP, in 2.5 use CV::CAP


cap.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U,794); // 736
cap.set(CV_CAP_PROP_WHITE_BALANCE_RED_V,437);
cap.set(CV_CAP_PROP_EXPOSURE,initShutter); // "Shutter" in coriander
cap.set(CV_CAP_PROP_FPS,fps);
cap.set(CV_CAP_PROP_GAMMA,0);
cap.set(CV_CAP_PROP_GAIN,30);




tmr0.setStart(); // Init it

cv::Rect roi = cv::Rect(0,0,640,360);

cv::Mat submatrix;
cv::Moments mu;
cv::Point2f mc;

cv::Scalar color = cv::Scalar( 0,255,0 );
float distX,distY;

cv::Point centerOfFrame;
cv::Size s;

Guppy g;

cameraLog cl;

long picNo =0;
std::ostringstream filen;

tmrFrame.setStart();

uint64_t old_tmr = 0;
uint64_t newtime = 0,difftmr = 0;


 double signal=0,nonLimitedSignal=0,pixeldist=0,radians=0,meters=0;


 double e =0;
 double initE =0;
 double initBase=0;  
 double initR =0;




while(runState){

 tmr0.setStop();
 
 cl.deltaus =  tmr0.elapsedTimeus();
 
 cl.idle = cl.deltaus-cl.processing;
 camLog.push_back(cl);
 
 if(!cameraError.ready && cl.deltaus < 20000 ){
   //it took less than 50 hz to get the frame. Camera should be up
   
   cameraError.ready = 1;
   signal = 0;
}

 tmr0.setStop();


 tmr0.setStart();
cap >> frame;

tmrFrame.setStop();

newtime = tmrFrame.elapsedTimeus();

difftmr = newtime-old_tmr;
old_tmr = newtime;

// cl.getFrame = tmr0.elapsedTimeus();
cl.getFrame = difftmr;


tmrProcessing.setStart();
//std::vector<std::vector<cv::Point> > contours;
//std::vector<cv::Vec4i> hierarchy;

/* Maybe */

//Remove gripper from img

// cv::Mat(frame,roi).copyTo(submatrix);
// 
// 
// // Get color image, decode bayer BGGR.  
// cv::cvtColor(submatrix,colorFrame,CV_BayerBG2RGB,0);
// cv::cvtColor(colorFrame, grey, CV_RGB2GRAY );
// 
// 
// tmr0.setStop();
// cl.colorConversion = tmr0.elapsedTimeus();
// 
// // Threshold it. The black dot becomes white. (NIBARY_INV)
// cv::threshold(grey,tresholdedFrame,cannyMin,255,cv::THRESH_BINARY_INV);

/* end maybe */



/* Before */

//Remove gripper from img

submatrix = cv::Mat(frame,roi);


// Get color image, decode bayer BGGR.  
cv::cvtColor(submatrix,colorFrame,CV_BayerBG2RGB);
cv::cvtColor(colorFrame, grey, CV_RGB2GRAY );


tmr0.setStop();
cl.colorConversion = tmr0.elapsedTimeus();

// Threshold it. The black dot becomes white. (NIBARY_INV)
cv::threshold(grey,tresholdedFrame,cannyMin,255,cv::THRESH_BINARY_INV);

// /* end BEFORE */

/* after* 
 * 
 **/

//  // Get color image, decode bayer BGGR.  
// cv::cvtColor(frame,colorFrame,CV_BayerBG2RGB,0);
// cv::cvtColor(colorFrame, grey, CV_RGB2GRAY );
// 
// submatrix = cv::Mat(grey,roi);
// 
// tmr0.setStop();
// cl.colorConversion = tmr0.elapsedTimeus();
// 
// 
// 
// 
// // Threshold it. The black dot becomes white. (NIBARY_INV)
// cv::threshold(grey,tresholdedFrame,cannyMin,255,cv::THRESH_BINARY_INV);

/* 
 * end after */


/* no bayer* 
 * 
 **/

 // Get color image, decode bayer BGGR.  
//cv::cvtColor(frame,colorFrame,CV_BayerBG2RGB,0);
//cv::cvtColor(colorFrame, grey, CV_RGB2GRAY );

// cv::Mat(frame,roi).copyTo(submatrix);;
// 
// tmr0.setStop();
// cl.colorConversion = tmr0.elapsedTimeus();
// 
// 
// 
// 
// // Threshold it. The black dot becomes white. (NIBARY_INV)
// cv::threshold(submatrix,tresholdedFrame,cannyMin,255,cv::THRESH_BINARY_INV);

/* 
 * end no bayer */


//cout << "Rows X Cols: " << frame.rows << " x " << frame.cols << endl;

/*string ty =  type2str( frame.type() );
printf("Matrix: %s %dx%d \n", ty.c_str(), frame.cols, frame.rows );

ty =  type2str( submatrix.type() );
printf("Matrix: %s %dx%d \n", ty.c_str(), submatrix.cols, submatrix.rows );
*/
// TRY: using raw image..
//submatrix = cv::Mat(frame,roi);



if(blockSize % 2 == 0){
//Adaptive threshold block size SKAL være ulige..
 
 blockSize = blockSize -1;
}

// cv::adaptiveThreshold(submatrix,tresholdedFrame,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY_INV,blockSize,0);

tmr0.setStop();
cl.thresholding = tmr0.elapsedTimeus();



mu = cv::moments(tresholdedFrame,true);


tmr0.setStop();
cl.moments = tmr0.elapsedTimeus();


// Find center
mc = cv::Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );

// Count non zero pixels. Used for determining if we are screwed (getting large "white" areas.)
cameraError.areaOfObject =  cv::countNonZero(tresholdedFrame);

tmr0.setStop();
cl.area = tmr0.elapsedTimeus();



// if(debugMonitor){ // Wierd stuff
//   // this is onlu needed if the users want to see whats going on.
//   
//   
//     cv::cvtColor(submatrix,colorFrame,CV_BayerBG2RGB,0);
//    // cv::cvtColor(colorFrame, grey, CV_RGB2GRAY );
//   
//     // Draw it - convert to RGB to we can draw on it with colors
//    // cv::cvtColor(tresholdedFrame, tresholdedFrame, CV_GRAY2RGB);
//     //cv::Mat drawing = cv::Mat::zeros( tresholdedFrame.size(), CV_8UC3 );
// 
//     cv::circle( colorFrame, mc, 5, color, -1, 8, 0 );
// 
// 
// }

 
// Calculate distance from center of image

s = tresholdedFrame.size(); 
centerOfFrame = cv::Point(s.width/2,s.height/2);  

distX = centerOfFrame.x-mc.x;
distY = centerOfFrame.y-mc.y;
 

if(mc.x != mc.x){
  // This fixes if mc.x or mc.y becomes NaN.
  //Should only! happen in camswing
  
  
  distX =-1;
  distY =-1;
  
}

cameraError.x = (int)distX;
cameraError.y = (int)distY; 


tmrProcessing.setStop();
cl.processing = tmrProcessing.elapsedTimeus();
tmrIdle.setStart();
// View part

  if(!frame .data) break;

  
  
  /*
   * 
   *  START TRANSMIT COMMAND
   * 
   * 
   */
  
  
/* SWING PART */

if(!controller){
      //base joint
    if(tmp.qActual[0] > 1.2){
      
      // keep swinging
      
    // signal = -0.1;
      signal = -1;
      
    }else{
      
      signal = 0;
    
    }
}

/* END SWING*/

/* ACTUAL CONTROLLER */
if(controller){

  if(cameraError.ready){
    // Only when ready!
    
    signal = cameraError.x;
pixeldist = signal;
    // from pixels to m
      signal = g.px2m(signal,g.getActualHeight(tmp.tool[2]));

      // Save initial error in meters
      
      if(!initE){
	initE = signal;
	//initR = 
	//cout << "du leder efter:" << initR << "baseret på: X " << tmp.tool[0] << "og Y: "<< tmp.tool[1] <<endl;
      }
      
      
meters = signal;
    // from m to radian
      
      signal = signal/g.getRadius(tmp.tool[0],tmp.tool[1]);
      
	
    radians = signal;
      
      // Apply controller...
      
    signal = signal*gain;
      
      nonLimitedSignal = signal;
    
      if(signal> 3.2){
	signal = 3.2;
      }else if(signal < -3.2){
	
	signal = -3.2;
      }
      
      
      
      
   /*   if(signal < 0.03 && signal > 0 ){
	signal = 0;
      }else if(signal > -0.03 && signal < 0){
	    signal = 0;
      }*/
  
  }
}
/* END ACTUAL CONTROLLER */


std::ostringstream strs;  

// Safety check is in the faster thread
  
if(!securityStop){
strs << "speedj([" << signal <<  ", 0, 0, 0, 0, 0],15,1)";
}
  
  
std::string cmd = strs.str();  


  if(!noRobot){
    
   
    
    if(cameraError.ready ==1 || securityStop){ // 20000 is OK, 16700 is OK, 
      
          
    robot.addCmd(cmd,0);
    robot.run();
       

      
    }
  
}


if(cameraError.ready){
  /* ADD TO LOG */
    if(cameraError.ready == 1) {
    robotLog.push_back(tmp);
    }


}
  
if(verbose){
  
cout << cmd << endl;
cout << setprecision(9) 
<< "pixeldist : " << cameraError.x
<< "; meter : " << tmp.cameraDistXm
<< "; radians : " << radians 
<<"; Non limited output : " << nonLimitedSignal 
<<", Limited: " << signal
<< ", AoO: "<< cameraError.areaOfObject << endl;

 
}
  
    
  /*
   * 
   *  END TRANSMIT COMMAND
   * 
   * 
   */
  
    if(histogram){
    
      histo =  g.histogramGS(grey,cannyMin);
	
      
      
      
      cv::imshow("Histogram",histo);
      
      if(!debugMonitor){
	if(cv::waitKey(1) >= 27){ break;  }
      }
	  
      
    }
  
  
if(debugMonitor){
  // this is onlu needed if the users want to see whats going on.
  
  
    // Draw it - convert to RGB to we can draw on it with colors
    cv::cvtColor(tresholdedFrame, tresholdedFrame, CV_GRAY2RGB);
    //cv::Mat drawing = cv::Mat::zeros( tresholdedFrame.size(), CV_8UC3 );

    cv::circle( tresholdedFrame, mc, 5, color, -1, 8, 0 );
    
     cv::cvtColor(grey, grey, CV_GRAY2RGB);
    cv::circle( grey, mc, 5, color, -1, 8, 0 );


}


  if(debugMonitor){
    // Show the iamge to the user
	cv::imshow("Thresholded",tresholdedFrame); // Uncomment this line to see the actual picture. It will give an unsteady FPS


      cv::imshow("Color",grey); // Uncomment this line to see the actual picture. It will give an unsteady FPS

      //cv::imshow( "Center", drawing );

      if(cv::waitKey(1) >= 27){ break;  } // We wait 1ms - so that the frame can be drawn. break on ESC

  }
  
/*  
    if(debugMonitor){ // Wierd stuff
    // Show the iamge to the user
	cv::imshow("Thresholded",tresholdedFrame); // Uncomment this line to see the actual picture. It will give an unsteady FPS


      cv::imshow("Color",colorFrame); // Uncomment this line to see the actual picture. It will give an unsteady FPS

      //cv::imshow( "Center", drawing );

      if(cv::waitKey(1) >= 27){ break;  } // We wait 1ms - so that the frame can be drawn. break on ESC

  }*/


if(saveImgs){
    // Draw the circle
   cv::cvtColor(grey, grey, CV_GRAY2RGB);
    cv::circle( grey, mc, 5, color, -1, 8, 0 );
  
  
    filen.str("");
    filen.clear();
    filen << "imgs/img" << setfill('0') << setw(5)  << picNo << ".ppm";
      std::string imgname = filen.str();
      
      cv::imwrite(imgname,grey);
      
      picNo++;
  }
  


}






cv::destroyWindow("Color"); //destroy the window with the name, "MyWindow"
cv::destroyWindow("Thresholded"); 

cout << "logfilename: " << logfilename << endl;
writeLog(robotLog,logfilename);
writeCameraLog(camLog,"camLog.txt");


cout << "Initial error[m]:" << initE <<endl;
cout << "Error at quit[rad]: "<< radians <<endl;
cout << "Initial radius[m]: "<< g.getRadius(tmp.tool[0],tmp.tool[1]) <<endl;

}

/* 
 * 
 * ROBOT TREAD BELOW
 * 
 * */


void* robotThread(void* arg)
{
 
  
  Timing timer0;
  
  Guppy g;
  
char buffer[rd1.getRecvLength()];
 double q[6];
 double qTarget[6];
 
 double nonLimitedSignal,pixeldist,radians,meters;


 
int counter = 0;

 timer0.setStart();
 
// double ref = 
 // double ref = 1.6;
 

 

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
  
  tmp.cameraDistXpx = cameraError.x;

  timer0.setStop();
 
 //timer0.setStop();
 
 rd1.setBuffer(buffer);

 

 rd1.getqActual(q);
 rd1.getqTarget(qTarget);
 
 
  tmp.robotTime = rd1.getTime(); // Returns double
  
 tmp.sysTime = timer0.elapsedTimeus();
  
 
  rd1.getqActual(tmp.qActual); // pass array pointer to store data.
 rd1.getqdActual(tmp.qdActual); // pass array pointer to store data.
 rd1.getqddTarget(tmp.qddTarget); // pass array pointer to store data.
 rd1.getqTarget(tmp.qTarget);
 rd1.getqdTarget(tmp.qdTarget);
 rd1.getTool(tmp.tool);
 
 
 // Check if rotation has added pi degrees?)
 if(fabs(tmp.qActual[0]) > 3){
   // 
   
   if(tmp.qActual[0] > 0){
      tmp.qActual[0] -= 2*M_PI;
     
  }else{
    tmp.qActual[0] += 2*M_PI;
    
  }
   
   
}
 
 
 
 tmp.cameraDistXm = g.px2m(tmp.cameraDistXpx,g.getActualHeight(tmp.tool[2]));

 
 
  
  /* 
   * 
   * WRITE DATA
   * 
   * */
  
std::ostringstream strs;  
  
/* Safety check and stop
 * - stops the robot if it tries to exit a defined areo of operation
 */

// Limit base operating area. First condition is to the "left"

if(tmp.qActual[0] < 0.863 || tmp.qActual[0] > 2.157 || securityStop){
  // Out of bounds. Override commands
  strs << "stop(15)";
  securityStop = true;
  
  
  stringstream ss;

ss << errMsg[1] << "@" << tmp.qActual[0];

 reason = ss.str();
  
}
else if(cameraError.areaOfObject > 200 ){

  // Probably seeing the table now
strs << "stop(15)";
securityStop = true;
stringstream ss;

ss << errMsg[0] << "@" << cameraError.areaOfObject;

 reason = ss.str();
}  

    
    
/* End Safety check and stop */ 





    


  if(!noRobot){
    
    sleeper.setStop();
    
    if(securityStop){ // 20000 is OK, 16700 is OK, 
      
     std::string cmd = strs.str();
    robot.addCmd(cmd,0);
    robot.run();
       
 sleeper.setStart();
      
    }

  
  }
  
  

  
   if(securityStop){
    
    cout << endl << "SECURITY STOPPED (soft) " << reason << endl;
    
  }
  
   /*struct timespec time1;
  time1.tv_sec =0;
  time1.tv_nsec = 7000000; // 8 ms / 125 hz
     */ 
   //nanosleep(&time1,NULL); // Lets sleep for 8ms/one tick
    
 


  
  
  
  counter++;
  
   
  }
  
  
   
  
  
}




  
void shutterCB(int pos, void* param){
struct timeval t;

cap.set(CV_CAP_PROP_EXPOSURE,pos);

//fcount=0; // Reset frame counter, so we dont have do wait for the avg to "catch" up



std::cout << "CALLBACK !!!: pos:  " << pos << "Shutter read: " << cap.get(CV_CAP_PROP_EXPOSURE) << std::endl;
}

  



