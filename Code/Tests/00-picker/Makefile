#Makefile 

LIBS  += $(shell pkg-config opencv --libs)
CXXFLAGS += $(shell pkg-config opencv --cflags)
CPPFLAGS =  -Wall -I/usr/include/opencv2

# Add code to to compiled here. eg test.cpp => test.o 
PROGS = picker.o logging.o  ../../Base/Network.o ../../Base/RobotCommander.o ../../Base/Guppy.o ../../Base/Gripper.o

# The Executable that will be made
OUT = picker 

all: $(PROGS) 
	g++ -c $(CPPFLAGS) $(CXXFLAGS) $(PROGS) -O3
	g++ $(PROGS) $(LIBS) -lcom_err -lpthread -O3 -o $(OUT)

	
debug: $(PROGS) 
	g++ -g3 -O0 $(CPPFLAGS) $(CXXFLAGS) $(PROGS) $(LIBS) -lcom_err -lpthread -o $(OUT)
# g++ -g $(PROGS) 
	
	
gripper: gripper_test.o
	g++ $(CPPFLAGS) $(CXXFLAGS) gripper_test.o ../../Base/Gripper.o -O3 -o gripper_test


clean : 
	rm -f *.o $(OUT)


