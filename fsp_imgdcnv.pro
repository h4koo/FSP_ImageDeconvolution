QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 optimize_full 
#optimize_full debug

QMAKE_CXXFLAGS += -pedantic -Wall -Wextra 
#-Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused #-Werror -Wold-style-cast

QMAKE_EXTRA_TARGETS += createdir

PRE_TARGETDEPS += createdir
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ./build/bin/
unix:OBJECTS_DIR = ./build/obj
unix:MOC_DIR = ./build/qt_moc
unix:UI_DIR = ./build/qt_ui

DATA_DIR = ~/.fsp_imgdcnv/

SOURCES += \
    main.cpp \
    ./src/UI/ImageDeconvolutionWindow.cpp \
    ./src/UI/FilterWindow.cpp \
    ./src/AppLogic/Filter.cpp \
    ./src/AppLogic/ImageLoader.cpp \
    ./src/AppLogic/FilterLogic.cpp \
    ./src/AppLogic/ImageDeconvolution.cpp \
    ./src/AppLogic/ImageCleaningLogic.cpp \
    ./src/AppLogic/VecImage.cpp \
    ./libs/libfrcima/src/libfrcima.cpp \
    

HEADERS += \
    ./include/UI/FilterWindow.h \
    ./include/UI/ImageDeconvolutionWindow.h \
    ./include/UI/uihelpers.h \
    ./src/AppLogic/Filter.hpp \
    ./src/AppLogic/ImageLoader.hpp \
    ./src/AppLogic/ImageDeconvolution.hpp \
    ./libs/libfrcima/include/libfrcima.hpp \
    ./include/AppLogic/VecImage.hpp \
    ./include/AppLogic/FilterInfo.hpp \
    ./include/AppLogic/ImageCleaningLogic.hpp \
    ./include/AppLogic/FilterLogic.hpp \
    ./libs/CImg/CImg.h

INCLUDEPATH = ./libs/CImg ./include/AppLogic ./include/UI ./libs/libfrcima/include ./src/AppLogic

LIBS += -L/usr/X11R6/lib -lm -lpthread -lX11 -ljpeg -larmadillo -lstdc++fs -lpng -lzip

createdir.commands = $(MKDIR) $$shell_path($$DATA_DIR) $$shell_path($$DESTDIR) $$shell_path($$OBJECTS_DIR) $$shell_path($$MOC_DIR) $$shell_path($$UI_DIR) 

FORMS += \
    ./include/UI/forms/filterwindow.ui \
    ./include/UI/forms/imagedeconvolutionwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
