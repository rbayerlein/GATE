bin = bin/ucdcoin2lm

CXX = g++
CXXFLAGS = -std=c++14

$(bin) : src/main.o
	$(CXX) $(CXXFLAGS) -o $(bin) src/main.o

main.o : src/main.cpp

.PHONY : clean

clean :
	-rm -v $(bin) src/main.o
