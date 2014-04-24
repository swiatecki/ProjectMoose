#ifndef GUPPY_H
#define GUPPY_H

#include <opencv2/core/core.hpp>


class Guppy{
  
    struct intrinsicParams{
   
   double x;
   double y;
   
};
  
public:
  Guppy();
  double px2m(int px,double height);
  double getActualHeight(double robotHeight);
  double getRadius(double robotX,double robotY);

cv::Mat histogramGS(cv::Mat image, int threshold) ;
  
  
private:
  intrinsicParams params;

  

};

#endif