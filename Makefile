CXX = g++
TARGET = glue
BINDIR	= /usr/local/bin
CXXFLAGS = -Wall -O2 --static -std=c++11
LDFLAGS = -lm
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)

all: $(TARGET) ./stdcom/eachline


$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.PHONY: clean test
clean:
	rm -f $(TARGET) $(OBJS)
	cd stdcom; make clean
	cd stdcom; make realclean

test: $(TARGET)
	sh -e ./TEST/test_all.sh

install: $(TARGET)
	cp $(CURDIR)/$(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm $(BINDIR)/$(TARGET) 
