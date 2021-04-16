all:
	$(MAKE) -C src

clean:
	$(MAKE) clean -C src

test: all
	sh -e ./test/test_all.sh

install:
	mkdir -p ~/.glue/bin/
	$(MAKE) install -C src
