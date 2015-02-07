CXX = g++
TARGET = glue
BINDIR	= /usr/local/bin
CXXFLAGS = -Wall -O2 --static -std=c++11
LDFLAGS = -lm
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
	mv $(TARGET) ../

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)

install: $(TARGET)
	cp $(CURDIR)/$(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm $(BINDIR)/$(TARGET) 
