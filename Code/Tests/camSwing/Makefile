#Makefile 

LIBS  += $(shell pkg-config opencv --libs)
CXXFLAGS += $(shell pkg-config opencv --cflags)
CPPFLAGS =  -Wall -I/usr/include/opencv2

# Add code to to compiled here. eg test.cpp => test.o 
PROGS = camSwing.o logging.o  ../../Base/Network.o ../../Base/RobotCommander.o ../../Base/Guppy.o

# The Executable that will be made
OUT = camSwing 

all: $(PROGS) 
	g++ -c $(CPPFLAGS) $(CXXFLAGS) $(PROGS) -O3
	g++ $(PROGS) $(LIBS) -lcom_err -lpthread -O3 -o $(OUT)

	
debug: $(PROGS) 
	g++ -c  $(CPPFLAGS) $(CXXFLAGS) -ggdb $(PROGS)
	g++  $(PROGS) $(LIBS) -lcom_err -lpthread -o $(OUT)


clean : 
	rm -f *.o $(OUT)


