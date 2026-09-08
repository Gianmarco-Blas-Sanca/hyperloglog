CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Iinclude

TARGET = app
SRCS = src/hyperloglog.cpp apps/app_main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)