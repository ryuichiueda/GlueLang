CXX = g++
TARGET = glue
BINDIR	= ~/.glue/bin/
CXXFLAGS = -Wall -O2 --static -std=c++17
LDFLAGS = -lm
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
	cp $(TARGET) ../

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
	rm -f $(TARGET)

install: $(TARGET)
	cp $(CURDIR)/$(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm $(BINDIR)/$(TARGET) 
