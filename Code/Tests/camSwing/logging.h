#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <core/types_c.h>
#include <stdint.h>

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

struct cameraLog{

uint64_t deltaus;
uint64_t getFrame;
uint64_t colorConversion;
uint64_t thresholding;
uint64_t moments;
uint64_t area;


cameraLog(){};
// Constructor, here using "initliazation list."
cameraLog(uint64_t _deltaus) : deltaus(_deltaus){};



};


typedef std::vector<cameraLog>::size_type cameraLog_sz;

void writeLog(std::vector<slogData> & data,std::string filename);

void writeCameraLog(std::vector<cameraLog> & data,std::string filename);

#endif