#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <string.h>
#include <math.h>

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



