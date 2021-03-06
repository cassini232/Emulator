/*
 * Write the UCSD pSystem IV boot and BIOS code to system tracks of drive A
 *
 * Copyright (C) 2008 by Udo Munk
 *
 * History:
 * 12-AUG-08 initial version
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>

/*
 *	This program writes the UCSD boot code from the following files
 *	onto the system tracks of the boot disk (drivea.cpm):
 *
 *	boot loader	boot.bin	(binary format)
 *	BIOS		bios.bin	(binary format)
 */
int main(void)
{
	unsigned char sector[128];
	register int i;
	int fd, drivea, readed;

	/* open drive A for writing */
	if ((drivea = open("../disks/drivea.cpm", O_WRONLY)) == -1) {
		perror("file ../disks/drivea.cpm");
		exit(1);
	}
	/* open boot loader (boot.bin) for reading */
	if ((fd = open("boot.bin", O_RDONLY)) == -1) {
		perror("file boot.bin");
		exit(1);
	}
	/* read boot loader */
	memset((char *) sector, 0, 128);
	read(fd, (char *) sector, 128);
	close(fd);
	/* and write it to disk in drive A */
	write(drivea, (char *) sector, 128);
	/* seek to sector 19 on drive A */
	lseek(drivea, (long) 18 * 128, 0);
	/* open BIOS (bios.bin) for reading */
	if ((fd = open("bios.bin", O_RDONLY)) == -1) {
		perror("file bios.bin");
		exit(1);
	}
	/* read BIOS from bios.bin and write it to disk in drive A */
	i = 0;
	while ((readed = read(fd, (char *) sector, 128)) == 128) {
		write(drivea, (char *) sector, 128);
		i++;
		if (i == 8) {
			puts("8 sectors written, can't write any more!");
			goto stop;
		}
	}
	if (readed > 0) {
		write(drivea, (char *) sector, 128);
	}
stop:
	close(fd);
	close(drivea);
	return(0);
}
