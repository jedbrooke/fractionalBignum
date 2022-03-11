CXX=g++
CFLAGS=--std=c++11 -Wall -Wextra

.PHONY: default
default: fractionalBignum.a

%.o: %.cpp
	$(CXX) -c $< $(CFLAGS)

fractionalBignum.o: fractionalBignum.hpp
	$(CXX) -o fractionalBignum.o -c fractionalBignum.hpp $(CFLAGS)

fractionalBignum.a: fractionalBignum.o utility.o
	ar rcs fractionalBignum.a fractionalBignum.o utility.o

	
basic_usage: fractionalBignum.a examples/basic_usage.cpp
	$(CXX) -o basic_usage examples/basic_usage.cpp -I. fractionalBignum.a $(CFLAGS)

.PHONY: clean
clean: 
	rm -rf *.a *.o