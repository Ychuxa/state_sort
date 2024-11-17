CC = gcc
FLAGS = -Wall -Wextra -Werror


all: build

build: state_sort

state_sort: state_sort.o
	$(CC) state_sort.o -o ../build/Quest_1

state_sort.o: state_sort.o
	$(CC) $(FLAGS) -c state_sort.c

