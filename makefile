#executable
run: prog.o S2DE.o
	gcc -Wall -o run prog.o S2DE.o -lglut -lGL -O2
	rm -f *.o



#program
prog.o: src/prog.c lib/S2DE.h
	gcc -Wall -c src/prog.c -O2



#S2DE management (use S2DE.o/.h to access to 2D graphic engine)
S2DE.o: lib/S2DE.c lib/S2DE.h
	gcc -Wall -c lib/S2DE.c -I/usr/include/GL -O2

