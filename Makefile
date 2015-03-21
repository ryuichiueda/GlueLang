all:
	$(MAKE) -C SRC

clean:
	cd SRC; make clean
	rm -f ./glue

test: all
	sh -e ./TEST/test_all.sh


install:
	cd SRC; make install
