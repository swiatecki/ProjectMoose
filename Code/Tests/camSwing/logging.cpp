#include "logging.h"

using namespace std;




void writeLog(std::vector<slogData> & data,std::string filename){

  

data_sz size = data.size();

ofstream a_file (filename.c_str(),ios::trunc);

a_file.precision(15);

for(data_sz i = 0; i != size; ++i){
  
  a_file<< data[i].robotTime << "," 
    << data[i].sysTime << "," 
    <<  data[i].qActual[0] << ","  // Actual joint pos
    <<  data[i].qActual[1] << "," 
     <<  data[i].qActual[2] << "," 
     <<  data[i].qActual[3] << "," 
     <<  data[i].qActual[4] << "," 
      <<  data[i].qActual[5] << "," 
    <<  data[i].qdActual[0] << "," // Actual joint velocity
    <<  data[i].qdActual[1] << "," 
    <<  data[i].qdActual[2] << "," 
    <<  data[i].qdActual[3] << "," 
    <<  data[i].qdActual[4] << "," 
    <<  data[i].qdActual[5] << ","
    <<  data[i].qddTarget[0] << "," // Target joint acc
    <<  data[i].qddTarget[1] << ","
    <<  data[i].qddTarget[2] << ","
    <<  data[i].qddTarget[3] << ","
    <<  data[i].qddTarget[4] << ","
    <<  data[i].qddTarget[5] << ","
    <<  data[i].qdTarget[0] << "," // Target joint velocity
    <<  data[i].qdTarget[1] << ","
    <<  data[i].qdTarget[2] << ","
    <<  data[i].qdTarget[3] << ","
    <<  data[i].qdTarget[4] << ","
    <<  data[i].qdTarget[5] << ","
    <<  data[i].qTarget[0] << "," // Target joint pos
    <<  data[i].qTarget[1] << "," 
    <<  data[i].qTarget[2] << "," 
    <<  data[i].qTarget[3] << "," 
    <<  data[i].qTarget[4] << "," 
    <<  data[i].qTarget[5] << ","
     <<  data[i].tool[0] << ","
    <<  data[i].tool[1] << ","
    <<  data[i].tool[2] << ","
    <<  data[i].tool[3] << ","
    <<  data[i].tool[4] << ","
    <<  data[i].tool[5] << ","
    <<  data[i].cameraDistXpx << ","
    <<  data[i].cameraDistXm << 
    endl;
  // Close the file stream explicitly


}

  a_file.close();



}