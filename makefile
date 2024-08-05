CC=g++
OBJS=Simulator.o LoadBalancer.o main.o
EXEC=simulator
DEBUG_FLAG= -g
COMP_FLAG=-std=c++17 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
Simulator.o : Simulator.hpp Simulator.cpp
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp
LoadBalancer.o : LoadBalancer.cpp LoadBalancer.hpp
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp
main.o: main.cpp Simulator.hpp
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp -o main.o

clean:
	rm -f $(OBJS) $(EXEC)
