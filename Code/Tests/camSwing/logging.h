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
double qddTarget[6];
double qdTarget[6];
double qTarget[6];
double tool[6];
int test01;
int cameraDistXpx;
double cameraDistXm;

};

typedef std::vector<slogData>::size_type data_sz;


void writeLog(std::vector<slogData> & data,std::string filename);

#endif