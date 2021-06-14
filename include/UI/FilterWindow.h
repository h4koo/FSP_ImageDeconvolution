/**
 * @file FilterWindow.h
 * @author Jorge Agüero Zamora
 * @brief Contains the `FilterWindow` class which holds the UI to create filters
 * @version 0.1
 * @date 2021-06-13
 * 
 */
#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>

#include <vector>
#include <string>
#include <iostream>

#include "uihelpers.h"
#include "ImageDeconvolutionWindow.h"
#include "FilterLogic.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class FilterWindow; }
QT_END_NAMESPACE

using std::string;
using std::vector;

namespace UI {
/**
 * @brief Window to create filters
 * 
 */
class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Filter Window object
     * 
     * @param parent parent of the window
     */
    FilterWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Filter Window object
     * 
     */
    ~FilterWindow();


private slots:
    void on_btn_load_folder_clicked();
    void on_next_loaded_image_btn_clicked();
    void on_listWidget_loaded_images_currentRowChanged(int currentRow);
    void on_previous_loaded_image_btn_clicked();
    void on_calc_filter_btn_clicked();
    void on_actionFilter_image_triggered();
    void on_actionLoadImageFromZip_triggered();
    void on_btn_save_dirtyimg_clicked();
    void on_btn_save_all_dirtyimg_clicked();    
    void on_noise_selection_group_currentChanged(int index);    
    void on_btn_load_single_image_clicked();
    void on_slider_gaussiannoise_valueChanged(int value);    
    void on_slider_snpnoise_valueChanged(int value);    
    void on_slider_uniformnoise_valueChanged(int value);    
    void on_slider_riciannoise_valueChanged(int value);
    void on_filterCalculationCanceled();
    void on_filterCalculationFinished();
    void on_loadImagesFinished();
    void on_loadImagesCanceled();
    void on_saveAllDirtyImagesFinished();
    void on_saveAllDirtyImagesCanceled();
    void on_lineEdit_filter_loaded_images_textChanged(const QString &arg1);
    void on_filter_name_input_editingFinished();
    
private:

    AppLogic::FilterLogic filter_logic; //!< API to create filters
    Ui::FilterWindow *ui; //!< pointer to access most of the window widgets
    QFutureWatcher<bool> future_watcher_calc_filter; //!< waits for the filter calculation thread to finish
    QFutureWatcher<bool> bool_future_watcher; //!< used to wait for image saving to finish
    QFutureWatcher<vector<string>> future_watcher_load_images; //!< used to wait for image loading to finish
    QProgressDialog * progress_dialog_calc_filter; //!< window to show progress bar
    QImage image; //!< stores the loaded image
    QImage dirty_image; //!< stores image with noise applied
    QLabel *label_loaded_image; //!< label to show loaded image
    QLabel *label_dirty_image; //!< label to show noisy image

    AppLogic::noise_type_t selected_noise_type; //!< curent noise type selected
    size_t selected_noise_value; //!< current noise value

    /**
     * @brief sets the image names in the list to display to user
     * 
     * @param loaded_filenames names of the loaded images
     * @return true if images were added correctly
     * @return false if there was a problem adding images to the list
     */
    bool loadImageList(const vector<string> &loaded_filenames);

    /**
     * @brief shows the preview of the loaded image in the window
     * 
     * @param index index of the loaded image to preview
     * @return true if able to create image and show it
     * @return false if unable to create image, clears the image
     */
    bool setLoadedImage(const size_t index);

    /**
     * @brief shows the preview of the currently selected loaded image in the window with noise applied 
     * 
     * @return true  if able to create image and show it
     * @return false if unable to create image, clears the image
     */
    bool setDirtyImage();

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
     * @brief returns the currently selected noise value for the noise type specified
     * 
     * @param noise_type the type of noise to get the value from
     * @return size_t value input by user for the specified noise type
     */
    size_t getSelectedNoiseValue(AppLogic::noise_type_t noise_type);

    /**
     * @brief saves currently displayed noisy image with the selected noise values
     * 
     * @param fileName name of the file to save the noisy image
     * @return true if image is saved successfully
     * @return false if image saving failed
     */
    bool saveDirtyImage(const QString &fileName);

    /**
     * @brief applies currently selected noise and value to loaded images and saves them to specified folder
     * 
     * This function will raise a progress dialog window while it waits for
     * images to finish loading
     * 
     * @param fileName path of the folder where the noisy images will be saved
     */
    void saveAllDirtyImages(const QString &fileName);

    /**
     * @brief creates a dialog window with the filter creation information
     * 
     */
    void createFilterInfoDialog();

    /**
     * @brief validates the input for the name of the filter
     * 
     * Checks whether the name exists or if it contains any illegal characters or device names that
     * would make the filename ilegal.
     * 
     * @return true if the name of the filter is valid
     * @return false if there is a problem with the filtername
     */
    bool validateNameInput();

    /**
     * @brief changes the color of the filter name input to indicate an error
     * 
     * @param error_message error message shown in status bar that explains problem with the filter name
     */
    void setFilterNameInputError(const QString &error_message);
};
}
#endif // FILTERWINDOW_H
