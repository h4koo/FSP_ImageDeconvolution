
INCL=-I../../libs/CImg/CImg -I../../include/AppLogic
LIBS=-L/usr/X11R6/lib -lm -lpthread -lX11 -ljpeg
OUT=../../test/bin/vec_im.test
SRC=VecImage.test.cpp ../../src/AppLogic/VecImage.cpp


test_vecimage:	
	g++ ${SRC} -g -o ${OUT} ${INCL} ${LIBS}