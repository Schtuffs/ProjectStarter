CXX		 = g++

EXE		 = CHANGEME

SRC		 = ../src
INCLUDE	 = ../include
LIB		 = ../lib

FLAGS	 = -std=c++20 -I$(INCLUDE) -L$(LIB)
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 = $(FLAGS) -o

OBJECTS	 = main.o

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

main.o: $(SRC)/main.cpp
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.exe *.o
