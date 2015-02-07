all:
	cd SRC; make
	cd stdcom; make

clean:
	cd SRC; make clean
	cd stdcom; make clean
	cd stdcom; make realclean
	rm -f ./glue

.PHONY: test
test: all
	sh -e ./TEST/test_all.sh

