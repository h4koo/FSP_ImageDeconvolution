(PHONY): clean

test_filter:
	cd src/AppLogic && make test_filter

test_imageloader:
	cd src/AppLogic && make test_imageloader

test_vecimage:	
	cd include/AppLogic && make test_vecimage

clean:
	rm -r ./build/obj/* ./build/bin/* ./test/bin/*
