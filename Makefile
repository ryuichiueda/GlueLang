all:
	$(MAKE) -C SRC
	$(MAKE) -C stdcom

clean:
	cd SRC; make clean
	cd stdcom; make clean
	cd stdcom; make realclean
	rm -f ./glue

test: all
	sh -e ./TEST/test_all.sh


install:
	cd SRC; make install
