CXX      = g++

EXE		 = WindowCreator

SRC		 = ../src
INCLUDE	 = ../include

FLAGS	 = -std=c++17 -I$(INCLUDE) -L../lib
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 = $(FLAGS)

OBJECTS	 = glad.o main.o Window.o Colour.o Callbacks.o

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -lglfw3dll -o $(EXE)

glad.o: $(SRC)/glad.c
	$(CXX) $(CXXFLAGS) $<

main.o: $(SRC)/main.cpp Window.o Colour.o $(INCLUDE)/Defines.h
	$(CXX) $(CXXFLAGS) $<

Window.o: $(SRC)/Window.cpp $(INCLUDE)/Window.h Colour.o $(INCLUDE)/Defines.h
	$(CXX) $(CXXFLAGS) $<

Colour.o: $(SRC)/Colour.cpp $(INCLUDE)/Colour.h $(INCLUDE)/Defines.h
	$(CXX) $(CXXFLAGS) $<

Callbacks.o: $(SRC)/Callbacks.cpp $(INCLUDE)/Callbacks.h
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.o *.exe

