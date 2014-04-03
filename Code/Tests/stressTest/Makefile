#Makefile 

LIBS  += $(shell pkg-config opencv --libs)
CXXFLAGS += $(shell pkg-config opencv --cflags)
CPPFLAGS =  -Wall -I/usr/include/opencv2

# Add code to to compiled here. eg test.cpp => test.o 
PROGS = stressTest.o logging.o  ../../Base/Network.o ../../Base/RobotCommander.o

# The Executable that will be made
OUT = stressTest 

all: $(PROGS) 
	g++ -c -g $(CPPFLAGS) $(CXXFLAGS) $(PROGS) -O0
	g++ -g $(PROGS) $(LIBS) -lcom_err -lpthread -O0 -o $(OUT)




clean : 
	rm -f *.o $(OUT)


