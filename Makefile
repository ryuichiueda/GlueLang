CXX = g++
TARGET = main
CXXFLAGS = -Wall -O3 --static -std=c++11
LDFLAGS = -lm
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o) #SRCSの各ファイルのサフィックスの.ccを.oに変換

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) 

clean:
	rm -f $(TARGET) $(OBJS)
