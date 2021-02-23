#executable
run: prog.o S2DE.o PNG.o
	gcc -Wall -o run prog.o S2DE.o PNG.o -lglut -lGL -lpng -O2
	rm -f *.o



#program
prog.o: prog.c S2DE.h PNG/PNG.h
	gcc -Wall -c prog.c -O2



#PNG library
PNG.o: PNG/PNG.c PNG/PNG.h
	gcc -Wall -c PNG/PNG.c -O2



#S2DE management (use S2DE.o/.h to access to 2D graphic engine)
S2DE.o: S2DE.c S2DE.h
	gcc -Wall -c S2DE.c -I/usr/include/GL -O2

