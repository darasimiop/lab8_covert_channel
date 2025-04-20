CXX = g++
CXXFLAGS = -O2

all: sender receiver

sender: sender.c util.cpp
	$(CXX) $(CXXFLAGS) -o sender sender.c util.cpp

receiver: receiver.c util.cpp
	$(CXX) $(CXXFLAGS) -o receiver receiver.c util.cpp

clean:
	rm -f sender receiver
