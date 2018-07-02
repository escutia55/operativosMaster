all: procesos procesosVM createswap swap

procesos: procesos.c mmu.o pagefault.o
	gcc -o procesos procesos.c mmu.o pagefault.o

procesosVM: procesos.c mmu.o pagefaultVM.o
	gcc -o procesosVM procesos.c mmu.o pagefaultVM.o

pagefault.o: pagefault.c mmu.h
	gcc -c pagefault.c

pagefaultVM.o: pagefaultVM.c mmu.h
	gcc -c pagefaultVM.c

mmu.o: mmu.c semaphores.h mmu.h
	gcc -c mmu.c

createswap: createswap.c
	gcc -o createswap createswap.c
	
clean:
	rm createswap mmu.o pagefault.o pagefaultVM.o procesos procesosVM

swap:
	./createswap
