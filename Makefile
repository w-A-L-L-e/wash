INSTALL_DIR = /usr/local/bin/

all:
	$(MAKE) -C src
	mv src/wash .

clean:
	@rm -vf wash
	$(MAKE) -C src clean

install:
	cp wash $(INSTALL_DIR)

uninstall:
	@rm -rf /usr/local/bin/wash



