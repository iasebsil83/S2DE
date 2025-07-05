#generate all
all: S2DE_pipe
	rm *.o






# ---------------- MAIN PROGRAM ----------------

#executable
S2DE_pipe: S2DE_pipe.o S2DE.o threads.o queues.o
	gcc -Wall -o S2DE_pipe S2DE_pipe.o S2DE.o threads.o queues.o -lglut -lGL -pthread -O2

#program
S2DE_pipe.o: src/S2DE_pipe.c lib/S2DE.h lib/threads.h lib/queues.h
	gcc -Wall -c src/S2DE_pipe.c -O2






# ---------------- LIBRARIES ----------------

#S2DE management (use S2DE.o/.h to access to 2D graphic engine)
S2DE.o: lib/S2DE.c lib/S2DE.h
	gcc -Wall -c lib/S2DE.c -I/usr/include/GL -O2

processes.o: lib/processes.c lib/processes.h
	gcc -Wall -c lib/processes.c -O2

threads.o: lib/threads.c lib/threads.h
	gcc -Wall -c lib/threads.c -O2

queues.o: lib/queues.c lib/queues.h
	gcc -Wall -c lib/queues.c -O2
