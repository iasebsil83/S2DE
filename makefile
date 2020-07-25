exe: prog.o S2DE.o
	gcc -Wall -o exe prog.o S2DE.o -lm -lglut -lGL -lX11
	rm -f *.o



#program
prog.o: prog.c S2DE.h
	gcc -Wall -O2 -c prog.c



#S2DE management (use S2DE.o/.h to access to graphic engine)
S2DE.o: S2DE.c S2DE.h
	gcc -Wall -O2 -c S2DE.c -I/usr/include/GL

