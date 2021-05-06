#ifndef IMAGEDECONVOLUTIONWINDOW_H
#define IMAGEDECONVOLUTIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class ImageDeconvolutionWindow;
}

class ImageDeconvolutionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageDeconvolutionWindow(QWidget *parent = nullptr);
    ~ImageDeconvolutionWindow();

private:
    Ui::ImageDeconvolutionWindow *ui;
};

#endif // IMAGEDECONVOLUTIONWINDOW_H
