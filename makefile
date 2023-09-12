#generate all
all: S2DE_bipipe techt
	rm *.o






# ---------------- MAIN PROGRAM ----------------

#executable
S2DE_bipipe: S2DE_bipipe.o S2DE.o bipipes.o processes.o threads.o queues.o
	gcc -Wall -o S2DE_bipipe S2DE_bipipe.o S2DE.o bipipes.o processes.o threads.o queues.o -lglut -lGL -pthread -O2

#program
S2DE_bipipe.o: src/S2DE_bipipe.c lib/S2DE.h lib/bipipes.h lib/processes.h lib/threads.h lib/queues.h
	gcc -Wall -c src/S2DE_bipipe.c -O2






# ---------------- EXAMPLE PEER ----------------

#executable
techt: techt.o bipipes.o threads.o
	gcc -o techt techt.o bipipes.o threads.o -pthread -Wall -O2

#program
techt.o: src/techt.c lib/bipipes.h lib/threads.h
	gcc -c src/techt.c -Wall -O2






# ---------------- LIBRARIES ----------------

#S2DE management (use S2DE.o/.h to access to 2D graphic engine)
S2DE.o: lib/S2DE.c lib/S2DE.h
	gcc -Wall -c lib/S2DE.c -I/usr/include/GL -O2

bipipes.o: lib/bipipes.c lib/bipipes.h
	gcc -Wall -c lib/bipipes.c -O2

processes.o: lib/processes.c lib/processes.h
	gcc -Wall -c lib/processes.c -O2

threads.o: lib/threads.c lib/threads.h
	gcc -Wall -c lib/threads.c -O2

queues.o: lib/queues.c lib/queues.h
	gcc -Wall -c lib/queues.c -O2
