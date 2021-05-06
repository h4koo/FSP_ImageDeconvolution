
INCL=-I../../libs/CImg/CImg -I../../include/AppLogic -I../../libs/libfrcima/include
APP_INCL=-I../../include/AppLogic
LIBS=-L/usr/X11R6/lib -lm -lpthread -lX11 -ljpeg -larmadillo -lstdc++fs
COMP_FLAGS=-std=c++17 -Wall -Wpedantic
# IL_SRC=ImageLoader.test.cpp ImageLoader.cpp
IL_SRC=$(OBJ_FOLDER)/ImageLoader.test.o $(OBJ_FOLDER)/ImageLoader.o $(OBJ_FOLDER)/VecImage.o
OBJ_FOLDER=../../build/obj

F_SRC=Filter.test.cpp Filter.cpp ImageLoader.cpp ../../libs/libfrcima/src/libfrcima.cpp VecImage.cpp

L_OUT=../../test/bin/im_ld.test
F_OUT=../../test/bin/filter.test

_FOBJ=Filter.test.o Filter.o ImageLoader.o libfrcima.o VecImage.o
FILTER_OBJ=$(patsubst %,$(OBJ_FOLDER)/%,$(_FOBJ))

$(OBJ_FOLDER)/%.o: %.cpp
	g++ -g -c -o $@ $< $(INCL) $(COMP_FLAGS)

$(OBJ_FOLDER)/libfrcima.o : ../../libs/libfrcima/src/libfrcima.cpp
	g++ -c -o $@ $< $(INCL) $(COMP_FLAGS)

test_imageloader:${IL_SRC}
	g++ ${IL_SRC} -o ${L_OUT} ${INCL} ${APP_INCL} ${LIBS} ${COMP_FLAGS} 

test_filter: $(FILTER_OBJ)
	g++ -g $(FILTER_OBJ) -o ${F_OUT} ${INCL} ${APP_INCL} ${LIBS} ${COMP_FLAGS}
