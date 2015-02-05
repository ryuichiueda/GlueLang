#
# Copyright (c) 2015 Mitzyuki IMAIZUMI  All rights reserved.
#

CFLAGS	= -g -Wall -Werror -I. -I$(INCDIR) -O2 

BASENAME  = librtscan
MAJOR     = 1
MINOR     = 0
SHLIB     = $(BASENAME).so.$(MAJOR).$(MINOR)
STATICLIB = $(BASENAME).a

PROGRAM = eachline

eachline: eachline.o 
	$(CC) $(CFLAGS) -o $@ $^ 

tags:
	ctags *.[ch]

clean:
	rm -f tags *.o 

realclean: clean
	rm -f $(PROGRAM)
