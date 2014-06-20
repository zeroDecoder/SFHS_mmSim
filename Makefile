CC=g++
CFLAGS=-Wall -Wno-unused-result -ansi -O2 -std=c++11 -g
LIBS=-lopencv_core -lopencv_features2d -lopencv_highgui

TARGET=main
HEADERS=gui.h mazeBase.h mazeConst.h studentAi.h
SOURCES=mmSim.cpp

all:	$(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)