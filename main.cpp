#include "filterwindow.h"
#include "imagedeconvolutionwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageDeconvolutionWindow img_dcv;
    img_dcv.show();
    return a.exec();
}
