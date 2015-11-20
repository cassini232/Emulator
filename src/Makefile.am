## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = emulator

emulator_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	code.c \
	flags.c \
	memory.c \
	general.c \
	log.c \
	registers.c \
	ports.c  \
	disas.c \
	iosim.c \
	sim1.c sim2.c sim3.c sim4.c sim5.c sim6.c sim7.c \
	simctl.c \
	simfun.c \
	simglb.c simglb.h \
	guiglb.c guiglb.h \
	sim.h \
	simint.c

emulator_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)
