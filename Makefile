CXX = g++
CXXFLAGS = -Wall -pthread

TARGET = chat
OBJS = main.o chat.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp chat.h
	$(CXX) $(CXXFLAGS) -c main.cpp

chat.o: chat.cpp chat.h
	$(CXX) $(CXXFLAGS) -c chat.cpp

clean:
	rm -f $(OBJS) $(TARGET)
