INSTALL_DIR = /bin/

all:
	$(MAKE) -C src
	mv src/wash .

clean:
	@rm -vf wash
	$(MAKE) -C src clean

install:
	cp wash $(INSTALL_DIR)



