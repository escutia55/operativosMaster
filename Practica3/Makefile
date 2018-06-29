all:	pi	pi_clones	pi_threads	pi_fork

pi:	pi.c
	gcc	-o	pi	pi.c

pi_clones: pi_clones.c
	gcc -o pi_clones pi_clones.c

pi_threads: pi_threads.c
	gcc -o pi_threads pi_threads.c -lpthread

pi_fork: pi_fork.c 
	gcc -o pi_fork pi_fork.c