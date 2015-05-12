# Compiling for Unix: make
# Compiling for Windows: make CROSS=i386-mingw32-

CC=$(CROSS)g++
LD=$(CROSS)ld
AR=$(CROSS)ar

selsim: main.o rng.o param.o seq.o pop.o sample.o
	$(CC) -O3 -o selsim main.o rng.o param.o seq.o pop.o sample.o
main.o: main.cpp rng.h param.h seq.h pop.h sample.h
	$(CC) -O3 -c main.cpp 
rng.o: rng.cpp rng.h 
	$(CC) -O3 -c rng.cpp 	
param.o: param.cpp param.h 
	$(CC) -O3 -c param.cpp 
seq.o: seq.cpp seq.h 
	$(CC) -O3 -c seq.cpp 
pop.o: pop.cpp pop.h 
	$(CC) -O3 -c pop.cpp 	
sample.o: sample.cpp sample.h 
	$(CC) -O3 -c sample.cpp 	
clean: 
	rm *.o selsim
