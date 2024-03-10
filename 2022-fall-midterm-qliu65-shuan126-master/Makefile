CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -lm
LIBS += -lm -g

project: ppm_io.o image_manip.o project.o
	$(CC) -o project project.o ppm_io.o image_manip.o $(LIBS)

project.o: project.c ppm_io.h image_manip.h
	$(CC) $(CFLAGS) -c project.c

ppmtesting: ppm_io_testing.o ppm_io.o image_manip.o
	$(CC) -o ppmtesting ppm_io_testing.o ppm_io.o image_manip.o $(LIBS)

ppm_io_testing.o: ppm_io_testing.c ppm_io.h 
	$(CC) $(CFLAGS) -c ppm_io_testing.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

image_manip.o: image_manip.c ppm_io.h
	$(CC) $(CFLAGS) -c image_manip.c

compare: img_cmp.o ppm_io.o
	$(CC) -o compare img_cmp.o ppm_io.o

img_cmp.o: img_cmp.c ppm_io.h
	$(CC) $(CFLAGS) -c img_cmp.c

checkerboard: checkerboard.o ppm_io.o
	$(CC) -o checkerboard checkerboard.o ppm_io.o

checkerboard.o: checkerboard.c ppm_io.h
	$(CC) $(CFLAGS) -c checkerboard.c

clean:
	rm -f *.o checkerboard project compare ppmtesting