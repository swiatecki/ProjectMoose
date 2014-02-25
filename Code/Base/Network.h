#ifndef NETWORK_H
#define NETWORK_H


//Target host details: (ROBOT )
#define PORT 30003
#define HOST "192.38.66.249"

class Network{
  
public:
  void startNet();
  void stopNet();
  int getSocket();
  
private:
  int sd;
  
};

#endif