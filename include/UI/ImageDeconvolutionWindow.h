/**
 * @file ImageDeconvolutionWindow.h
 * @author Jorge Agüero Zamora
 * @brief Contains `ImageDeconvolutionWindow` class which holds the UI to apply filters to images
 * @version 0.1
 * @date 2021-06-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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

#include "FilterWindow.h"

namespace Ui
{
    class ImageDeconvolutionWindow;
}

namespace UI {
using AppLogic::FilterInfo;
using std::string;
using std::vector;

/**
 * @brief Window to filter images 
 * 
 */
class ImageDeconvolutionWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Image Deconvolution Window object
     * 
     * @param parent parent of the window
     */
    explicit ImageDeconvolutionWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Image Deconvolution Window object
     * 
     */
    ~ImageDeconvolutionWindow();

/**
 * @brief functions called whenever there is an event on the window
 * 
 * These methods are self explanatory from the name.
 * 
 */
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
    Ui::ImageDeconvolutionWindow *ui; //!< pointer to access most of the window widgets
    AppLogic::ImageCleaningLogic img_cleaning; //!< API to filter images
    QImage image; //!< stores the loaded image
    QImage filtered_image; //!< stores the image with filter applied
    QProgressDialog *progress_dialog_filter; //!< window to show progress bar
    QFutureWatcher<bool> bool_future_watcher; //!< used to wait for image saving to finish
    QFutureWatcher<bool> loadfilter_future_watcher; //!< used to wait for filters to load
    QFutureWatcher<vector<string>> future_watcher_load_images; //!< used to wait for image loading to finish
    QLabel *label_loaded_image; //!< label to show loaded image
    QLabel *label_filtered_image; //!< label to show filtered image
    bool is_filter_loaded; //!< label to show loaded image
    bool changed_filter_loading; //!< indicates whether selected filter changed while loading previous filter

    /**
     * @brief sets the image names in the list to display to user
     * 
     * @param loaded_filenames names of the loaded images
     * @return true if images were added correctly
     * @return false if there was a problem adding images to the list
     */
    bool loadImageList(const std::vector<string> &loaded_filenames);

    /**
     * @brief shows the preview of the loaded image in the window
     * 
     * @param index index of the loaded image to preview
     * @return true if able to create image and show it
     * @return false if unable to create image, clears the image
     */
    bool setLoadedImage(const size_t index);

    /**
     * @brief shows the preview of the currently selected loaded image in the window with filter applied 
     * 
     * @param index index of the image to filter
     * @return true  if able to create image and show it
     * @return false if unable to create image, clears the image
     */
    bool setFilteredImage(const size_t index);

    /**
     * @brief loads images from specified folder and shows them in the window
     * 
     * This function will raise a progress dialog window while it waits for
     * images to finish loading
     * 
     * @param folder_path path of the folder where the images reside
     */
    void loadImagesFromFolder(const QString &folder_path);

    /**
     * @brief load images from a zip file
     * 
     * This function will raise a progress dialog window while it waits for
     * images to finish loading
     * 
     * 
     * @param folder_path full path of a valid zip file
     */
    void loadImagesFromZip(const QString &folder_path);

    /**
     * @brief load list of images provided
     * 
     * @param filenames list of paths of images to load
     * @return true if images are loaded correctly
     * @return false if unable to load images
     */
    bool loadImagesFromFile(const QStringList &filenames);

    /**
     * @brief saves currently displayed filter image
     * 
     * @param fileName name of the file to save the filtered image
     * @return true if image is saved successfully
     * @return false if image saving failed
     */
    bool saveFilteredImage(const QString &fileName);

    /**
     * @brief applies currently selected filter to loaded images and saves them to specified folder
     * 
     * This function will raise a progress dialog window while it waits for
     * images to finish saving
     * 
     * @param fileName path of the folder where the filtered images will be saved
     */
    void saveAllFilteredImages(const QString &fileName);

    /**
     * @brief updates the list of loaded filters
     * 
     */
    void updateFilterList();

    /**
     * @brief displays the filter information on the window
     * 
     * @param fi `FilterInfo` object containing the information about the filter
     */
    void displayFilterInfo(AppLogic::FilterInfo *fi);

    /**
     * @brief imports filter from a zip export file
     * 
     * @param import_filename path to the zip file export
     */
    void importFilter(const QString &import_filename);

    /**
     * @brief exports currently selected filter as a zip file
     * 
     * @param export_filename 
     */
    void exportFilter(const QString &export_filename);

};

}

#endif // IMAGEDECONVOLUTIONWINDOW_H
