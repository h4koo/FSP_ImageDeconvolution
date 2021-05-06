#include "filterwindow.h"
#include "imagedeconvolutionwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FilterWindow w;
    ImageDeconvolutionWindow img_dcv;
    w.show();
    img_dcv.show();
    return a.exec();
}
