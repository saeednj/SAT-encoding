DIR = ../core

all:
	g++ -std=c++11 -c $(DIR)/formula.cpp -o $(DIR)/formula.o
	g++ -std=c++11 gen_amo_espresso.cpp $(DIR)/formula.o -I $(DIR) -o gen_amo_espresso

clean:
	rm -f gen_amo_espresso $(DIR)/formula.o
