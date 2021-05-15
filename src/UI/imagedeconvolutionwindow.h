#ifndef IMAGEDECONVOLUTIONWINDOW_H
#define IMAGEDECONVOLUTIONWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QtWidgets>
#include <vector>
#include <string>
#include <iostream>

#include "uihelpers.h"
#include "ImageCleaningLogic.hpp"

namespace Ui {
class ImageDeconvolutionWindow;
}

class ImageDeconvolutionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageDeconvolutionWindow(QWidget *parent = nullptr);
    ~ImageDeconvolutionWindow();

private slots:
    void on_next_loaded_image_btn_clicked();

    void on_previous_loaded_image_btn_clicked();

    void on_listWidget_loaded_images_currentRowChanged(int currentRow);

    void on_load_images_btn_clicked();

    void on_listWidget_loaded_images_itemClicked(QListWidgetItem *item);

    void on_comboBox_filter_select_currentIndexChanged(int index);

    void on_pushButton_filter_image_clicked();

private:
    Ui::ImageDeconvolutionWindow *ui;
    AppLogic::ImageCleaningLogic img_cleaning;

    QImage image;
    QImage filtered_image;

//    AppLogic::Filter filter;
    QLabel * label_loaded_image;
    QLabel * label_filtered_image;

    bool loadFolder(const QString &fileName);
    bool setLoadedImage(const QString &fileName);
    bool setFilteredImage(const QString &fileName);
};

#endif // IMAGEDECONVOLUTIONWINDOW_H
