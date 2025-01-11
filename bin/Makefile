CXX		= g++

EXE		= proj

SRC		= ../src
INCLUDE	= ../include
LIB		= ../lib

FLAGS	= -std=c++20 -I$(INCLUDE) -L$(LIB)
CXXFLAGS= -c -Wall $(FLAGS)
LDFLAGS	= $(FLAGS) -o

OBJECTS	= main.o FileStructure.o

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

main.o: $(SRC)/main.cpp FileStructure.o
	$(CXX) $(CXXFLAGS) $<

FileStructure.o: $(SRC)/FileStructure.cpp $(INCLUDE)/FileStructure.h
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.exe *.o

