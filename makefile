CXX=g++
CFLAGS=-Wall -Wextra

%.o: %.cpp
	$(CXX) -c $< $(CFLAGS)
	
basic_usage: fractionalBignum.hpp examples/basic_usage.cpp utility.o
	$(CXX) -o basic_usage examples/basic_usage.cpp utility.o -I. $(CFLAGS)