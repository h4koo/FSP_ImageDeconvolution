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

class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    FilterWindow(QWidget *parent = nullptr);
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

    AppLogic::FilterLogic filter_logic;
    Ui::FilterWindow *ui;
    QFutureWatcher<bool> future_watcher_calc_filter;
    QFutureWatcher<bool> bool_future_watcher;
    QFutureWatcher<vector<string>> future_watcher_load_images;
    QProgressDialog * progress_dialog_calc_filter;
    QImage image;
    QImage dirty_image;
    QRegExp rx;
    QRegExpValidator * filename_validator_regexp;
    QLabel *label_loaded_image;
    QLabel *label_dirty_image;

    AppLogic::noise_type_t selected_noise_type;
    size_t selected_noise_value;

    bool loadImageList(const vector<string> &loaded_filenames);
    bool setLoadedImage(const size_t index);
    bool setDirtyImage();

    void loadImagesFromFolder(const QString &folder_path);
    void loadImagesFromZip(const QString &folder_path);
    bool loadImagesFromFile(const QStringList &filenames);    
    size_t getSelectedNoiseValue(AppLogic::noise_type_t noise_type);
    bool saveDirtyImage(const QString &fileName);
    void saveAllDirtyImages(const QString &fileName);
    void createFilterInfoDialog();
    bool validateNameInput();
    void setFilterNameInputError(const QString &error_message);
};
#endif // FILTERWINDOW_H
