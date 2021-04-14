all:
	$(MAKE) -C src
	$(MAKE) -C utils

clean:
	$(MAKE) clean -C src
	$(MAKE) clean -C utils

test: all
	sh -e ./test/test_all.sh

install:
	$(MAKE) install -C src
	$(MAKE) install -C utils
