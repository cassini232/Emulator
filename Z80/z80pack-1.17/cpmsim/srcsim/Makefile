CC = gcc

# Platform independend compiler optimization
#CFLAGS = -O3 -c -Wall

# Intel Pentium M (Centrino) and gcc 4.1.1
CFLAGS = -O3 -march=i686 -minline-all-stringops -funroll-loops -c -Wall

# AMD Opteron and gcc 3.3.5
#CFLAGS = -O3 -mcpu=i686 -minline-all-stringops -c -Wall

# Linux, BSD, Cygwin
LFLAGS = -s

# Solaris 9
#LFLAGS = -s -lsocket -lnsl -lrt

OBJ =   sim0.o \
	sim1.o \
	sim2.o \
	sim3.o \
	sim4.o \
	sim5.o \
	sim6.o \
	sim7.o \
	simctl.o \
	simint.o \
	iosim.o \
	simfun.o \
	simglb.o \
	unix_terminal.o

all: ../auxin ../auxout ../cpmsim
	@echo "done."

../auxin:
	test -f ../auxin || mknod ../auxin p

../auxout:
	test -f ../auxout || mknod ../auxout p

../cpmsim : $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o ../cpmsim

sim0.c:
	./lnsrc

sim0.o : sim0.c sim.h simglb.h
	$(CC) $(CFLAGS) sim0.c

sim1.o : sim1.c sim.h simglb.h
	$(CC) $(CFLAGS) sim1.c

sim2.o : sim2.c sim.h simglb.h
	$(CC) $(CFLAGS) sim2.c

sim3.o : sim3.c sim.h simglb.h
	$(CC) $(CFLAGS) sim3.c

sim4.o : sim4.c sim.h simglb.h
	$(CC) $(CFLAGS) sim4.c

sim5.o : sim5.c sim.h simglb.h
	$(CC) $(CFLAGS) sim5.c

sim6.o : sim6.c sim.h simglb.h
	$(CC) $(CFLAGS) sim6.c

sim7.o : sim7.c sim.h simglb.h
	$(CC) $(CFLAGS) sim7.c

simctl.o : simctl.c sim.h simglb.h
	$(CC) $(CFLAGS) simctl.c

simint.o : simint.c sim.h simglb.h
	$(CC) $(CFLAGS) simint.c

iosim.o : iosim.c sim.h simglb.h
	$(CC) $(CFLAGS) iosim.c

simfun.o : simfun.c sim.h
	$(CC) $(CFLAGS) simfun.c

simglb.o : simglb.c sim.h
	$(CC) $(CFLAGS) simglb.c

unix_terminal.o : ../../iodevices/unix_terminal.c
	$(CC) $(CFLAGS) ../../iodevices/unix_terminal.c

clean:
	rm -f *.o
	./ulnsrc

allclean:
	make clean
	rm -f ../auxin ../auxout ../cpmsim ../cpmsim.exe
