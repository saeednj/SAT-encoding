SRCS = ../core/formula.cpp util.cpp sha1.cpp sha256.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main
CFLAGS = -std=c++11 -O2 -I ../core/
MACROS = #-D_DOTMATRIX_ #-D_COMPRESSOR_TEST

CFLAGS += $(MACROS)

.cpp.o: $(SRCS)
	g++ -c $< -o $@ $(CFLAGS)

all: $(OBJS) $(TARGET).cpp
	g++ $(OBJS) $(CFLAGS) $(TARGET).cpp -o $(TARGET)

verify: verify-sha1.cpp verify-sha256.cpp
	g++ $(OBJS) $(CFLAGS) verify-sha1.cpp -o verify-sha1
	g++ $(OBJS) $(CFLAGS) verify-sha256.cpp -o verify-sha256

clean:
	rm -f $(OBJS) $(TARGET) verify-sha1 verify-sha256
