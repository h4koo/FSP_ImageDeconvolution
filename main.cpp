/**
 * @file main.cpp
 * @author Jorge Agüero Zamora
 * @brief main entry point for application
 * @version 0.1
 * @date 2021-06-14
 * 
 */
#include "ImageDeconvolutionWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UI::ImageDeconvolutionWindow img_dcv;
    img_dcv.show();
    return a.exec();
}
