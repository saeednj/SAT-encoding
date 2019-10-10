SRCS = ../core/formula.cpp long.cpp mod.cpp ppa.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main
CFLAGS = -std=c++11 -O2 -I ../core/

.cpp.o: $(SRCS)
	g++ -c $< -o $@ $(CFLAGS)

all: $(OBJS) $(TARGET).cpp
	g++ $(OBJS) $(CFLAGS) $(TARGET).cpp -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
