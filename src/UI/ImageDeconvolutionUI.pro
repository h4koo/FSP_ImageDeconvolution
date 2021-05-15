QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 debug

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../../test/bin/UI
unix:OBJECTS_DIR = ../../build/obj
unix:MOC_DIR = ../../build/qt_moc
unix:UI_DIR = ../../build/qt_ui

SOURCES += \
    imagedeconvolutionwindow.cpp \
    main.cpp \
    filterwindow.cpp \
    ../AppLogic/Filter.cpp \
    ../AppLogic/ImageLoader.cpp \
    ../AppLogic/FilterLogic.cpp \
    ../AppLogic/ImageDeconvolution.cpp \
    ../AppLogic/ImageCleaningLogic.cpp \
    ../AppLogic/VecImage.cpp \
    ../../libs/libfrcima/src/libfrcima.cpp \
    

HEADERS += \
    filterwindow.h \
    imagedeconvolutionwindow.h \
    uihelpers.h \
    ../AppLogic/Filter.hpp \
    ../AppLogic/ImageLoader.hpp \
    ../AppLogic/ImageDeconvolution.hpp \
    ../../libs/libfrcima/include/libfrcima.hpp \
    ../../include/AppLogic/VecImage.hpp \
    ../../include/AppLogic/FilterInfo.hpp \
    ../../include/AppLogic/ImageCleaningLogic.hpp \
    ../../include/AppLogic/FilterLogic.hpp \
    ../../libs/CImg/CImg/CImg.h

INCLUDEPATH = ../../libs/CImg/CImg ../../include/AppLogic ../../libs/libfrcima/include ../AppLogic

LIBS += -L/usr/X11R6/lib -lm -lpthread -lX11 -ljpeg -larmadillo -lstdc++fs

FORMS += \
    filterwindow.ui \
    imagedeconvolutionwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
