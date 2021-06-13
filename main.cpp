#include "FilterWindow.h"
#include "ImageDeconvolutionWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageDeconvolutionWindow img_dcv;
    img_dcv.show();
    return a.exec();
}
