CC = g++ -g


all: lab6

lab6.o: lab6.cpp
	$(CC) -c lab6.cpp


output_generator.o: output_generator.cpp
	$(CC) -c output_generator.cpp

# Link the object files into the final executable lab6
lab6: lab6.o output_generator.o
	$(CC) -o lab6 lab6.o output_generator.o

clean:
	rm -f lab6 lab6.o output_generator.o
