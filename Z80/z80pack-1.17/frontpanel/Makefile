SRCS = jpeg.cpp lpanel.cpp lp_gfx.cpp lp_main.cpp lp_utils.cpp \
	lp_window.cpp lp_switch.cpp lp_font.cpp

OBJS = $(SRCS:.cpp=.o)

CC = g++

# compiler switches

INCLUDE =
DEFINES = 

CFLAGS = -c -O3 -fPIC -Wno-deprecated $(INCLUDE) $(DEFINES) $(LIB)

default: $(OBJS) 
	g++ -shared $(LDFLAGS) $(OBJS) -o libfrontpanel.so

jpeg.o: jpeg.cpp cdjpeg.h
	$(CC) $(CFLAGS) jpeg.cpp -o jpeg.o

lpanel.o: lpanel.cpp lpanel.h lp_utils.h
	$(CC) $(CFLAGS) lpanel.cpp -o lpanel.o

lp_font.o: lp_font.cpp lp_font.h
	$(CC) $(CFLAGS) lp_font.cpp -o lp_font.o

lp_gfx.o: lp_gfx.cpp lpanel.h lp_utils.h jpeg.h
	$(CC) $(CFLAGS) lp_gfx.cpp -o lp_gfx.o

lp_main.o: lp_main.cpp lp_main.h lpanel.h lp_utils.h
	$(CC) $(CFLAGS) lp_main.cpp -o lp_main.o

lp_switch.o: lp_switch.cpp lpanel.h lp_switch.h lp_utils.h
	$(CC) $(CFLAGS) lp_switch.cpp -o lp_switch.o

lp_utils.o: lp_utils.cpp lp_utils.h 
	$(CC) $(CFLAGS) lp_utils.cpp -o lp_utils.o

lp_window.o: lp_window.cpp lpanel.h 
	$(CC) $(CFLAGS) lp_window.cpp -o lp_window.o

clean:
	rm -f *.o

allclean: clean
	rm -f libfrontpanel.so

all: clean default
