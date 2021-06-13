QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 debug
QMAKE_EXTRA_TARGETS += createdir

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
    ./src/UI/imagedeconvolutionwindow.cpp \
    ./src/UI/filterwindow.cpp \
    ./src/AppLogic/Filter.cpp \
    ./src/AppLogic/ImageLoader.cpp \
    ./src/AppLogic/FilterLogic.cpp \
    ./src/AppLogic/ImageDeconvolution.cpp \
    ./src/AppLogic/ImageCleaningLogic.cpp \
    ./src/AppLogic/VecImage.cpp \
    ./libs/libfrcima/src/libfrcima.cpp \
    

HEADERS += \
    ./include/UI/filterwindow.h \
    ./include/UI/imagedeconvolutionwindow.h \
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

createdir.commands = $(MKDIR) $$shell_path($$DATA_DIR)

FORMS += \
    ./include/UI/forms/filterwindow.ui \
    ./include/UI/forms/imagedeconvolutionwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
