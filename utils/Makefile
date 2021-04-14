all: invout

invout: invout.c
	gcc -O3 -o invout -Wall invout.c

clean:
	rm -f invout

install: invout 
	mkdir -p /usr/local/bin/glueutils/
	cp invout /usr/local/bin/glueutils/
