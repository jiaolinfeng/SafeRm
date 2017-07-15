CC = g++
FLAGS = -Wall -O3

.PHONY : all clean
all : safe_rm.cpp
	$(CC) $(FLAGS) -o srm safe_rm.cpp 
	mv srm /usr/local/bin
	@echo "Make Done"

clean:
	@-rm srm
	
