CC=g++
CFLAGS=-Wall -ansi -O2 -lopencv_core -lopencv_highgui -std=c++11

TARGET=main
HEADERS=gui.h mazeBase.h mazeConst.h studentAi.h
SOURCES=mmSim.cpp

all:	$(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)