#ifndef GUPPY_H
#define GUPPY_H



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


  
  
private:
  intrinsicParams params;

  

};

#endif