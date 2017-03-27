all:
	$(MAKE) -C src
	cd manual_src; make

clean:
	cd src; make clean
	rm -f ./glue

test: all
	sh -e ./test/test_all.sh


install:
	cd src; make install
