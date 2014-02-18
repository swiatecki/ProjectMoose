#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#ifndef LOGGING_H
#define LOGGING_H

struct slogData{

double robotTime;
unsigned long sysTime;
double qActual[6];
double qdActual[6];


};

typedef std::vector<slogData>::size_type data_sz;


void writeLog(std::vector<slogData> & data,std::string filename);

#endif