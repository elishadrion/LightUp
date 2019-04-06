G++ = g++ 
FLAGS=-Wall -g -std=c++17

all: lightup 

lightup: lightup.o Solver.o
	$(G++) $(FLAGS) *.o -o lightup

Solver.o: Solver.cpp Solver.hpp SolverTypes.hpp Global.hpp Heap.hpp
	$(G++) $(FLAGS) -c Solver.cpp 

lightup.o: Main.cpp  Solver.hpp 
	$(G++) $(FLAGS) -c Main.cpp  -o lightup.o

clean:
	$(RM) *.o lightup
