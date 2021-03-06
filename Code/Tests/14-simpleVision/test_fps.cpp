#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "test02.cpp"
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/time.h>
#include <fstream>




struct hsvRange{

int hmax,hmin,smax,smin,vmax,vmin;

} colorRange;

struct fpsData{

int renderTime;
double fps, fpsAvg;

};

std::vector<fpsData> outData;

typedef std::vector<fpsData>::size_type fps_sz;


int fps = 60;
//int delay = ((double)(1/(double)(fps)))*1000;
//Prototypes
void readCapParams();
void shutterCB(int pos, void* param);
void onMouse( int event, int x, int y, int, void* );
void writeLog();
///Globals

cv::VideoCapture cap;  


//Theese two are global so that the callback can modify them 
long fcount = 0; 
unsigned long t_us_start; 

//
int main(int argc, char** argv){

//for timing

struct timeval t;
unsigned long t_us_now,t_us_done,t_diff,t2_us_start,t2_us_stop,t2_diff,t2_temp;
double fps_calc =0;
double fps_avg =0;

// HSV range
colorRange.hmin = 0;
colorRange.hmax = 5;
colorRange.smin = 0;
colorRange.smax = 5;
colorRange.vmin = 230;
colorRange.vmax = 255;

// For color/treshold sliders
//cv::Scalar g_tres_min(128,128,128);
//cv::Scalar g_tres_max(170,170,170);

//unsigned int colH = 128,colS = 128,colV
 
//TestClass tc;

//tc.doStuff();
 


 
cv::Mat frame; 
std::cout << "Init test3" << std::endl;

cv::namedWindow("Color", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"


int initShutter = 194;


int shutterVal = initShutter;

// Shutter slider
cv::createTrackbar("tbShutter","Color",&shutterVal,4095,shutterCB,NULL);


cv::Mat colorFrame;
cv::Mat tresholdedFrame;
cv::Mat hsvFrame;

// HSV values are H from 0-180 while S and V are from 0-255 
//cv::createTrackbar("tbH","MyWindow",&colH,180,HCB,&g_tres_min);
//cv::createTrackbar("tbS","MyWindow",&colS,255,SCB,NULL);
//cv::createTrackbar("tbV","MyWindow",&colV,255,VCB,NULL);

cv::setMouseCallback("Color",onMouse,&colorFrame);

cap.open(CV_CAP_DC1394); // Open first firewire camera. in 2.3 use CV_CAP, in 2.5 use CV::CAP

//Get al config of camera
//readCapParams();




std::cout << "Initial FPS: " << cap.get(CV_CAP_PROP_FPS) << std::endl;
// Try setting one

cap.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U,794); // 736
cap.set(CV_CAP_PROP_WHITE_BALANCE_RED_V,437);
cap.set(CV_CAP_PROP_EXPOSURE,initShutter); // "Shutter" in coriander
cap.set(CV_CAP_PROP_FPS,fps);
cap.set(CV_CAP_PROP_GAMMA,1);
cap.set(CV_CAP_PROP_GAIN,30);


std::cout << "Set FPS: " << cap.get(CV_CAP_PROP_FPS) << "And gamma: " << cap.get(CV_CAP_PROP_GAMMA) << std::endl;


//readCapParams();


//initial time
gettimeofday(&t, NULL);
t_us_start = t.tv_sec*(1000000)+t.tv_usec;

t_us_done = t_us_start;


int cnt = -1;
int frameCnt = 0;
bool countingState=false;
while(1){

 
cap >> frame;

//gettimeofday(&t, NULL);
//t2_us_stop = t.tv_sec*(1000000)+t.tv_usec;



//Lets get time now
gettimeofday(&t, NULL);
t_us_now = t.tv_sec*(1000000)+t.tv_usec;


t_diff = t_us_now-t_us_done;

fps_calc = 1.0/((double)(t_diff)/1000000.0); // calc fps

fps_avg = fcount/((double)(t_us_now-t_us_start)/1000000.0); // calc avg fps
fcount++;

gettimeofday(&t, NULL);
t_us_done = t.tv_sec*(1000000)+t.tv_usec;

// Get color image, decode bayer BGGR.  

cv::cvtColor(frame,colorFrame,CV_BayerBG2RGB,0);
cv::cvtColor(colorFrame,hsvFrame,CV_RGB2HSV,0);

/* -------- TRESHOLDING ----------- */


if(!colorFrame.data) break;


// For filtered HSV
//cv::imshow("HSV",tresholdedFrame); // Uncomment this line to see the actual picture. It will give an unsteady FPS


//cv::imshow("Color",colorFrame); // Uncomment this line to see the actual picture. It will give an unsteady FPS


// Copy benchmark data to outData

fpsData tmp;

tmp.renderTime = (unsigned int)t_diff;
tmp.fps = fps_calc;
tmp.fpsAvg = fps_avg;

outData.push_back(tmp);


std::cout << "Exec time (us): "<< t_diff << " Calc FPS: " << fps_calc << ", FPS(avg): " << fps_avg << std::endl;






if(cv::waitKey(1) >= 2){break;} // We wait 1ms - so that the frame can be drawn

frameCnt++; // LED frame count

}


cv::destroyWindow("Color"); //destroy the window with the name, "MyWindow"
cv::destroyWindow("HSV"); 

// output framerates to file 

writeLog();


return 0;
}

void readCapParams(){


double mode = 99;
for(int i = -4;i<=21;i++){


mode = cap.get(i);
std::cout << "Param nr  " << i << " har: " << mode << std::endl;
mode = 99;

}




}

void shutterCB(int pos, void* param){
struct timeval t;

cap.set(CV_CAP_PROP_EXPOSURE,pos);

fcount=0; // Reset frame counter, so we dont have do wait for the avg to "catch" up

gettimeofday(&t, NULL);
t_us_start = t.tv_sec*(1000000)+t.tv_usec;

std::cout << "CALLBACK !!!: pos:  " << pos << "Shutter read: " << cap.get(CV_CAP_PROP_EXPOSURE) << std::endl;
}


void onMouse( int event, int x, int y, int, void* userdata ){
cv::Mat hsvMat;

if( event != cv::EVENT_LBUTTONDOWN ) {return;}
unsigned int R=0,G=0,B=0,H=0,S=0,V=0;

cv::Mat* RGB = (cv::Mat*) userdata;
B = (*RGB).at<cv::Vec3b>(y,x)[0]; 
G = (*RGB).at<cv::Vec3b>(y,x)[1]; 
R = (*RGB).at<cv::Vec3b>(y,x)[2]; 

// Convert to HSV
cv::cvtColor((*RGB),hsvMat,CV_RGB2HSV);

H = hsvMat.at<cv::Vec3b>(y,x)[0]; 
S = hsvMat.at<cv::Vec3b>(y,x)[1]; 
V = hsvMat.at<cv::Vec3b>(y,x)[2]; 


std::cout << "(x,y):" << x << "," << y << " (R,G,B): " << R << "," << G << "," << B << " - (H,S,V) " << H << "," << S << "," << V << std::endl;

}


void writeLog(){

fps_sz size = outData.size();

std::ofstream a_file ("fpsLog.txt",std::ios::trunc);


for(std::vector<fpsData>::size_type i = 0; i != size; ++i){

 
  a_file<< outData[i].renderTime << "," << outData[i].fps << "," << outData[i].fpsAvg << std::endl;
  // Close the file stream explicitly


}

  a_file.close();



}
