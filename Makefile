CXX = g++
TARGET = glue
CXXFLAGS = -Wall -O3 --static -std=c++11
LDFLAGS = -lm
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.PHONY: clean test
clean:
	rm -f $(TARGET) $(OBJS)

test:
	sh -e ./TEST/test_all.sh
