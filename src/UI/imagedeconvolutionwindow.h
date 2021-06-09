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

#include "filterwindow.h"

namespace Ui {
class ImageDeconvolutionWindow;
}

using std::string;
using std::vector;
using AppLogic::FilterInfo;

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
    void on_comboBox_filter_select_currentIndexChanged(int index);
    void on_pushButton_save_filterimage_clicked();    
    void on_pushButton_filterall_save_clicked();    
    void on_btn_load_image_clicked();
    void on_actionCreate_filter_triggered();
    void on_actionLoadImageFromZip_triggered();
    void on_btn_load_folder_clicked();
    void on_pushButton_deletefilter_clicked();    
    void on_pushButton_exportfilter_clicked();    
    void on_pushButton_importfilter_clicked();
    void on_filterLoadingFinished();
    // void on_filter_text_changed();
    void on_lineEdit_filter_loaded_images_textChanged(const QString &arg1);
    void on_filterAllImagesCanceled();
    void on_filterAllImagesFinished();
    void on_loadImagesFinished();
    void on_loadImagesCanceled();
    void on_exportFilterFinished();
    void on_exportFilterCanceled();
    void on_importFilterFinished();
    void on_importFilterCanceled();

private:
    Ui::ImageDeconvolutionWindow *ui;
    AppLogic::ImageCleaningLogic img_cleaning;

    QImage image;
    QImage filtered_image;
    QProgressDialog *progress_dialog_filter;

    //    AppLogic::Filter filter;
    QFutureWatcher<bool> bool_future_watcher;
    QFutureWatcher<bool> loadfilter_future_watcher;
    QFutureWatcher<vector<string>> future_watcher_load_images;
    QLabel * label_loaded_image;
    QLabel * label_filtered_image;    
    bool is_filter_loaded;

    bool loadImageList(const std::vector<string> &loaded_filenames);
    bool setLoadedImage(const size_t index);
    bool setFilteredImage(const size_t index);
    void loadImagesFromFolder(const QString &folder_path);
    bool loadImagesFromFile(const QStringList &filenames);
    bool saveFilteredImage(const QString &fileName);
    void saveAllFilteredImages(const QString &fileName);
    void updateFilterList();
    void displayFilterInfo(AppLogic::FilterInfo *fi);
    void loadImagesFromZip(const QString &folder_path);
    void importFilter(const QString &import_filename);
    void exportFilter(const QString &export_filename);
};

#endif // IMAGEDECONVOLUTIONWINDOW_H
