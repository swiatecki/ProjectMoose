#ifndef GRIPPER_H
#define GRIPPER_H


//Target host details: (ROBOT )
#define PORT 31101
#define HOST "192.38.66.233"

class Gripper{
  
public:
  void startGripper();
  void stopGripper();
  void setGripper(int dist);
  
private:
  int sock;
  
};

#endif