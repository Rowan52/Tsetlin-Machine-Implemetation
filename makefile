#XORDemo: XORDemo.cpp TM4.o
#	g++ XORDemo.cpp TM4.o -o Program.exe
	
#XORDemo: XORDemo.cpp TM4.o
#	g++ -o $@ XORDemo.cpp TM4.o

#TM4.o: TM4.cpp
#	g++ -o $@ -c TM4.cpp

XORDemo: XORDemo.cpp TM4.cpp TM4.h
	g++ -Wall -ffast-math -o XORDemo XORDemo.cpp TM4.cpp TM4.h -lm

clean:
	rm *.o XORDemo
