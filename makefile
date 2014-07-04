CC = gcc
CFLAGS = -Wall -g 

all: pareto est_a est_a_newton pareto.o

pareto: pareto.c
	${CC} ${CFLAGS} -DSTANDALONE -o pareto pareto.c

pareto.o: pareto.c
	${CC} ${CFLAGS} -c pareto.c

est_a: est_a.c input.o input.h
	${CC} ${CFLAGS} -o est_a est_a.c input.o

est_a_newton: est_a_newton.c input.o input.h
	${CC} ${CFLAGS} -o est_a_newton est_a_newton.c input.o

input.o: input.c input.h
	${CC} ${CFLAGS} -c input.c

clean:
	rm -f *.o pareto est_a est_a_newton core

