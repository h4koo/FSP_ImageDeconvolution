#include "imagedeconvolutionwindow.h"
#include "ui_imagedeconvolutionwindow.h"

ImageDeconvolutionWindow::ImageDeconvolutionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageDeconvolutionWindow)
{
    ui->setupUi(this);
}

ImageDeconvolutionWindow::~ImageDeconvolutionWindow()
{
    delete ui;
}
