#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QtWidgets>
#include <vector>
#include <string>
#include <iostream>

#include "uihelpers.h"
#include "FilterLogic.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class FilterWindow; }
QT_END_NAMESPACE

class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    FilterWindow(QWidget *parent = nullptr);
    ~FilterWindow();

private slots:
    void on_load_images_btn_clicked();

    void on_listWidget_loaded_images_itemClicked(QListWidgetItem *item);



    void on_next_loaded_image_btn_clicked();

    void on_listWidget_loaded_images_itemEntered(QListWidgetItem *item);

    void on_listWidget_loaded_images_currentRowChanged(int currentRow);

    void on_previous_loaded_image_btn_clicked();

    void on_calc_filter_btn_clicked();
    void on_gaussian_slider_sliderReleased();

private:

    AppLogic::FilterLogic filter_logic;
    Ui::FilterWindow *ui;    
    QImage image;
    QImage dirty_image;
//    AppLogic::Filter filter;

    QLabel *label_loaded_image;
    QLabel *label_dirty_image;

    bool loadFolder(const QString &fileName);
    bool setLoadedImage(const QString &fileName);
    bool setDirtyImage(const QString &fileName);
//    static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
};
#endif // FILTERWINDOW_H
