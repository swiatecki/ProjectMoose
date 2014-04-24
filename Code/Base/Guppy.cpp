#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <string.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "Guppy.h"



using namespace std;




Guppy::Guppy(){

    params.x = 779.28;
  params.y = 779.28;
}


double Guppy::px2m(int px,double height)
{

  double tmp;
  
  
  tmp = ((double)(px/params.x))*height;

  return tmp;
}


double Guppy::getActualHeight(double robotHeight)
{

  //camera is approx 14cm lower than gripper. 
  // table is 14 cm heigher than robot offset.
  return robotHeight-0.27;
  
  
}


double Guppy::getRadius(double robotX, double robotY )
{
  double tmp;
  
  tmp = pow(robotX,2)+pow(robotY,2);
  
  return sqrt(tmp); 
  
}

cv::Mat Guppy::histogramGS(cv::Mat image)
{
  /* FROM
   * http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
   * */
  
int histSize = 256;

/// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };
  
bool uniform = true; bool accumulate = false;

    
cv::Mat _hist;

cv::calcHist(&image, 1, 0, cv::Mat(), _hist, 1, &histSize, &histRange, uniform, accumulate );


int hist_w = 512; int hist_h = 400;
int bin_w = cvRound( (double) hist_w/histSize );
  


cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
  

cv::normalize(_hist, _hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );



/// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(_hist.at<float>(i-1)) ) ,
                       cv::Point( bin_w*(i), hist_h - cvRound(_hist.at<float>(i)) ),
                       cv::Scalar( 255, 0, 0), 2, 8, 0  );
  
  }

return histImage;
}

