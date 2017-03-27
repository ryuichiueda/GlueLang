all:
	$(MAKE) -C src

clean:
	cd src; make clean
	rm -f ./glue

test: all
	sh -e ./test/test_all.sh


install:
	cd src; make install
