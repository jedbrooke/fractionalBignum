CXX=g++
CFLAGS=-Wall -Wextra
	
basic_usage: fractionalBignum.hpp examples/basic_usage.cpp 
	$(CXX) -o basic_usage examples/basic_usage.cpp fractionalBignum.o -I. $(CFLAGS)