all: opencv

opencv:
	 $(MAKE) --directory=src

install:
	$(MAKE) --directory=src install

clean:
	$(MAKE) --directory=src clean
