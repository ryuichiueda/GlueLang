CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11
SRCS = test
OBJS := $(SRCS:.cc=.o)

all: test

test: test.cc
	$(CXX) $(CXXFLAGS) test.cc -o test

clean:
	rm -f $(TARGET) $(OBJS) 
