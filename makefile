CXX=g++
CFLAGS=--std=c++11 -Wall -Wextra -O2

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

speed_comparison: fractionalBignum.a examples/speed_comparison.cpp
	$(CXX) -o speed_comparison examples/speed_comparison.cpp -I. fractionalBignum.a -lgmp -lgmpxx $(CFLAGS)

.PHONY: clean
clean: 
	rm -rf *.a *.o