CXX      = g++

DLL		 = CHANGEME.dll
ARC		 = libCHANGEMEdll.a

SRC		 = ../src
INCLUDE	 = ../include

FLAGS	 = -std=c++23 -I$(INCLUDE) -L../lib
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 =  $(FLAGS) -o
DLLFLAGS = -I$(INCLUDE) -DDLL -DEXAMPLE_EXPORTS -o

OBJECTS	 = Example.o
SOURCE	 = $(SRC)/Example.cpp

dll: $(OBJECTS)
	ar rcs $(ARC) $(OBJECTS)
	$(CXX) $(DLLFLAGS) $(DLL) $(SOURCE)

Example.o: $(SRC)/Example.cpp $(INCLUDE)/Example.h
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.o *.dll *.a

