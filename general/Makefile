SRCS = formula.cpp util.cpp sha1.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main
CFLAGS = -std=c++11 -O2

.cpp.o: $(SRCS)
	g++ -c $< -o $@ $(CFLAGS)

all: $(OBJS) $(TARGET).cpp
	g++ $(OBJS) $(CFLAGS) $(TARGET).cpp -o $(TARGET)

verify: verify-sha1.cpp
	g++ $(OBJS) $(CFLAGS) verify-sha1.cpp -o verify-sha1

clean:
	rm -f $(OBJS) $(TARGET) verify-sha1
