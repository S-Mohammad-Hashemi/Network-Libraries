# All compilations occur with -Wall to print warnings, and -g to allow gdb debugging.
# The clean command uses rm to remove all expendable files (rm is part of
# the cs1300 compiler tools from www.cs.colorado.edu/~main/cs1300/README.html).

# define the different outputs of all g++ commands
# on Ubuntu, the .exe suffix disappears
EXPENDABLES = driver1 driver2 driver3

# make all is listed first, which lets you type "make" to recompile 
# anything that has changed in either your class, your test code, 
# or my test code.
all:
	@make $(EXPENDABLES)

# this compiles your test program
driver1: driver1.cpp
	g++ -std=c++11 -o driver1 driver1.cpp ThreadPool.cpp -pthread
	
driver2: driver2.cpp
	g++ -std=c++11 -o driver2 driver2.cpp EventScheduler.cpp -pthread
	
driver3: driver3.cpp
	g++ -std=c++11 -o driver3 driver3.cpp  message.h 

	
	
	
# this deletes all the output files
clean:
	rm $(EXPENDABLES)
