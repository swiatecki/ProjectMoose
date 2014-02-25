#Makefile 

#LIBS  += $(shell pkg-config opencv --libs)
#CXXFLAGS += $(shell pkg-config opencv --cflags)
CPPFLAGS =  -Wall 

# Add code to to compiled here. eg test.cpp => test.o 
PROGS = main_program.cpp logging.cpp Network.cpp

# The Executable that will be made
OUT = main_program 

all: $(PROGS) 
	g++ -c -g $(CPPFLAGS) $(CXXFLAGS) $(PROGS) -O0 
	g++  -g $(PROGS) $(LIBS) -lpthread -lcom_err -O0 -o $(OUT)


clean : 
	rm -f *.o $(OUT)

