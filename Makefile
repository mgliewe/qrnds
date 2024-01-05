
all:
	make -C src all
	make -C composer all

clean:
	make -C src clean
	make -C composer clean

distclean:
	make -C src distclean
	make -C composer distclean


run: all
	cd composer; node server.js
