#include "FilterWindow.h"
#include "ui_filterwindow.h"

#define WINDOWS_DEVICES "CON|AUX|PRN|COM1|COM2|LPT1|LPT2|NUL"
#define SLASHES "/\\"

namespace UI
{
    using AppLogic::noise_type_t;
    using std::cout;
    using std::endl;
    using std::string;
    using std::vector;

    FilterWindow::FilterWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::FilterWindow), label_loaded_image(new QLabel),
          label_dirty_image(new QLabel), selected_noise_type(noise_type_t::GAUSSIAN), selected_noise_value(0)
    {
        ui->setupUi(this);

        //create label to load image
        this->label_loaded_image->setBackgroundRole(QPalette::Base);
        this->label_loaded_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        this->label_loaded_image->setScaledContents(true);

        //create label to show dirty/filtered image
        this->label_dirty_image->setBackgroundRole(QPalette::Base);
        this->label_dirty_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        this->label_dirty_image->setScaledContents(true);

        //set up the label inside the scroll area
        ui->loaded_image_area->setBackgroundRole(QPalette::Dark);
        ui->loaded_image_area->setWidget(this->label_loaded_image);

        //set up the label inside the scroll area
        ui->dirty_image_area->setBackgroundRole(QPalette::Dark);
        ui->dirty_image_area->setWidget(this->label_dirty_image);
        ui->noise_selection_group->setCurrentIndex(0);

        connect(&this->future_watcher_calc_filter, SIGNAL(finished()), this, SLOT(on_filterCalculationFinished()));
    }

    FilterWindow::~FilterWindow()
    {
        delete ui;
    }

    void FilterWindow::on_btn_load_folder_clicked()
    {
        QFileDialog dialog(this, tr("Open folder with images load"));
        uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
        if (dialog.exec() == QDialog::Accepted)
        {
            loadImagesFromFolder(dialog.selectedFiles().first());
        }
    }

    void FilterWindow::on_loadImagesFinished()
    {
        this->progress_dialog_calc_filter->setMaximum(100);
        this->progress_dialog_calc_filter->setValue(100);
        vector<string> loaded_filenames = this->future_watcher_load_images.result();
        if (loaded_filenames.empty())
        {
            statusBar()->showMessage(tr("Unable to load images"), LONGER_MESSAGE_TIME);
            return;
        }
        this->loadImageList(loaded_filenames);
        statusBar()->showMessage(tr("Loaded %1 images").arg(loaded_filenames.size()), LONGER_MESSAGE_TIME);
    }

    void FilterWindow::on_loadImagesCanceled()
    {
        AppLogic::ImageLoader::cancelOperation(true);
    }

    void FilterWindow::loadImagesFromFolder(const QString &folder_path)
    {
        QFuture<vector<string>> image_loading = QtConcurrent::run(&this->filter_logic, &AppLogic::FilterLogic::loadImagesFromFolder, folder_path.toStdString());
        this->future_watcher_load_images.setFuture(image_loading);
        connect(&this->future_watcher_load_images, SIGNAL(finished()), this, SLOT(on_loadImagesFinished()));
        //create a progress dialog
        this->progress_dialog_calc_filter = new QProgressDialog(tr("Loading images"), tr("Stop"), 0, 0, this);
        this->progress_dialog_calc_filter->setWindowModality(Qt::WindowModal);
        connect(this->progress_dialog_calc_filter, SIGNAL(canceled()), this, SLOT(on_loadImagesCanceled()));
        this->progress_dialog_calc_filter->exec();
    }

    void FilterWindow::loadImagesFromZip(const QString &folder_path)
    {
        QFuture<vector<string>> image_loading = QtConcurrent::run(&this->filter_logic, &AppLogic::FilterLogic::loadImagesFromZip, folder_path.toStdString());
        this->future_watcher_load_images.setFuture(image_loading);
        connect(&this->future_watcher_load_images, SIGNAL(finished()), this, SLOT(on_loadImagesFinished()));

        //create a progress dialog
        this->progress_dialog_calc_filter = new QProgressDialog(tr("Loading images"), tr("Stop"), 0, 0, this);
        this->progress_dialog_calc_filter->setWindowModality(Qt::WindowModal);
        connect(this->progress_dialog_calc_filter, SIGNAL(canceled()), this, SLOT(on_loadImagesCanceled()));
        this->progress_dialog_calc_filter->exec();
    }

    bool FilterWindow::loadImagesFromFile(const QStringList &filenames)
    {
        this->filter_logic.clearWorkingImages();
        vector<string> loaded_filenames;
        for (auto &&filename : filenames)
        {
            loaded_filenames.push_back(this->filter_logic.addImageToWorkingImages(filename.toStdString()));
        }
        return loadImageList(loaded_filenames);
    }

    bool FilterWindow::loadImageList(const vector<string> &loaded_filenames)
    {
        ui->listWidget_loaded_images->clear();
        for (string item : loaded_filenames)
        {
            ui->listWidget_loaded_images->addItem(QString(item.c_str()));
        }
        //Set selected row to be first row and load image
        ui->listWidget_loaded_images->setCurrentRow(0);
        this->setLoadedImage(0);
        unsigned int img_size = this->image.height() * this->image.width();
        unsigned int max_rank = loaded_filenames.size() < img_size ? loaded_filenames.size() : img_size;
        ui->rank_value_spinBox->setRange(1, max_rank);
        ui->filter_name_input->setText("filter_");
        ui->label_min_max_rank->setText(tr("min: 1  max: %1").arg(max_rank));
        return true;
    }

    bool FilterWindow::setLoadedImage(const size_t index)
    {
        AppLogic::VecImage *vi = this->filter_logic.getLoadedImage(index);
        if (vi == NULL)
        {
            this->label_loaded_image->clear();
            return false;
        }
        const QImage f_image(vi->getRawImageData(), vi->numCols(), vi->numRows(), vi->numCols(), QImage::Format::Format_Grayscale8);
        if (f_image.isNull())
        {
            this->label_loaded_image->clear();
            return false;
        }
        this->image = f_image;
        this->label_loaded_image->setPixmap(QPixmap::fromImage(f_image));
        this->label_loaded_image->adjustSize();
        return true;
    }

    size_t FilterWindow::getSelectedNoiseValue(AppLogic::noise_type_t noise_type)
    {
        switch (noise_type)
        {
        case noise_type_t::GAUSSIAN:
            return static_cast<size_t>(ui->slider_gaussiannoise->value());
        case noise_type_t::SALT_PEPPER:
            return static_cast<size_t>(ui->slider_snpnoise->value());
        case noise_type_t::UNIFORM:
            return static_cast<size_t>(ui->slider_uniformnoise->value());
        case noise_type_t::RICIAN:
            return static_cast<size_t>(ui->slider_riciannoise->value());

        default:
            return 0;
        }
    }

    bool FilterWindow::setDirtyImage()
    {
        AppLogic::VecImage vi = this->filter_logic.applyNoiseToImage(ui->listWidget_loaded_images->currentRow(), this->selected_noise_type, this->selected_noise_value);

        //create QImage from noisy image
        const QImage d_image(vi.getRawImageData(), vi.numCols(), vi.numRows(), vi.numCols(), QImage::Format::Format_Grayscale8);
        if (d_image.isNull())
        {
            this->label_dirty_image->clear();
            return false;
        }
        this->dirty_image = d_image;
        this->label_dirty_image->setPixmap(QPixmap::fromImage(this->dirty_image));
        this->label_dirty_image->adjustSize();
        return true;
    }

    void FilterWindow::on_next_loaded_image_btn_clicked()
    {
        int current_row = ui->listWidget_loaded_images->currentRow();
        if (current_row < 0)
            return;
        if (current_row + 1 < ui->listWidget_loaded_images->count())
            ui->listWidget_loaded_images->setCurrentRow(current_row + 1);
    }

    void FilterWindow::on_listWidget_loaded_images_currentRowChanged(int currentRow)
    {
        this->setLoadedImage(currentRow);
        this->setDirtyImage();
    }

    void FilterWindow::on_previous_loaded_image_btn_clicked()
    {
        int current_row = ui->listWidget_loaded_images->currentRow();
        if (current_row < 0)
            return;
        if (current_row - 1 >= 0)
            ui->listWidget_loaded_images->setCurrentRow(current_row - 1);
    }

    void FilterWindow::on_calc_filter_btn_clicked()
    {
        if (!this->validateNameInput())
            return;
        string filter_name = ui->filter_name_input->text().toStdString(); //name of filter
        this->filter_logic.setNoise(this->selected_noise_type);
        this->filter_logic.setNoiseValue(this->selected_noise_value);
        int rank_val = ui->rank_value_spinBox->value(); // rank value
        AppLogic::calc_method_t selected_method = ui->rcima_rbtn->isChecked() ? AppLogic::calc_method_t::RCIMA_METHOD : AppLogic::calc_method_t::FAST_RCIMA_METHOD;
        //calculate filter in a thread
        QFuture<bool> calc_result = QtConcurrent::run(&this->filter_logic, &AppLogic::FilterLogic::createFilter, filter_name, rank_val, selected_method);
        this->future_watcher_calc_filter.setFuture(calc_result);
        //create a progress dialog
        this->progress_dialog_calc_filter = new QProgressDialog(tr("Calculating filter"), tr("Stop"), 0, 0, this);
        this->progress_dialog_calc_filter->setWindowModality(Qt::WindowModal);
        connect(this->progress_dialog_calc_filter, SIGNAL(canceled()), this, SLOT(on_filterCalculationCanceled()));
        this->progress_dialog_calc_filter->exec();
    }

    void FilterWindow::on_filterCalculationCanceled()
    {
        this->filter_logic.cancelFilterCreation();
    }

    void FilterWindow::on_filterCalculationFinished()
    {
        this->progress_dialog_calc_filter->setMaximum(100);
        this->progress_dialog_calc_filter->setValue(100);
        if (!this->future_watcher_calc_filter.result())
        {
            statusBar()->showMessage(tr("Filter calculation failed"), LONGER_MESSAGE_TIME);
            return;
        }
        this->createFilterInfoDialog();
    }

    void FilterWindow::on_slider_gaussiannoise_valueChanged(int value)
    {
        this->selected_noise_value = value;
        this->setDirtyImage();
    }

    void FilterWindow::on_slider_snpnoise_valueChanged(int value)
    {
        this->selected_noise_value = value;
        this->setDirtyImage();
    }

    void FilterWindow::on_slider_uniformnoise_valueChanged(int value)
    {
        this->selected_noise_value = value;
        this->setDirtyImage();
    }

    void FilterWindow::on_slider_riciannoise_valueChanged(int value)
    {
        this->selected_noise_value = value;
        this->setDirtyImage();
    }

    void FilterWindow::on_actionFilter_image_triggered()
    {
        ImageDeconvolutionWindow *img_decv = new ImageDeconvolutionWindow();
        img_decv->show();
        this->close();
    }

    void FilterWindow::on_btn_save_dirtyimg_clicked()
    {
        QFileDialog dialog(this, tr("Save noisy image as"));
        uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptSave, uihelp::file_dialog_type_t::FD_IMAGE, QFileDialog::FileMode::AnyFile);

        while (dialog.exec() == QDialog::Accepted && !saveDirtyImage(dialog.selectedFiles().first()))
        {
        }
    }

    bool FilterWindow::saveDirtyImage(const QString &fileName)
    {
        return this->filter_logic.saveDirtyImage(ui->listWidget_loaded_images->currentRow(), fileName.toStdString());
    }

    void FilterWindow::on_saveAllDirtyImagesFinished()
    {
        this->progress_dialog_calc_filter->setMaximum(100);
        this->progress_dialog_calc_filter->setValue(100);
        if (!this->bool_future_watcher.result())
        {
            statusBar()->showMessage(tr("Error saving noisy images"), LONGER_MESSAGE_TIME);
            return;
        }
        statusBar()->showMessage(tr("Finished saving noisy images"), STANDARD_MESSAGE_TIME);
    }

    void FilterWindow::on_saveAllDirtyImagesCanceled()
    {
        this->filter_logic.cancelSaveDirtyImages();
    }

    void FilterWindow::saveAllDirtyImages(const QString &fileName)
    {
        QFuture<bool> saving_images = QtConcurrent::run(&this->filter_logic, &AppLogic::FilterLogic::saveAllDirtyImages, fileName.toStdString());
        this->bool_future_watcher.setFuture(saving_images);
        connect(&this->bool_future_watcher, SIGNAL(finished()), this, SLOT(on_saveAllDirtyImagesFinished()));

        //create a progress dialog
        this->progress_dialog_calc_filter = new QProgressDialog(tr("Saving noisy images..."), tr("Stop"), 0, 0, this);
        this->progress_dialog_calc_filter->setWindowModality(Qt::WindowModal);
        connect(this->progress_dialog_calc_filter, SIGNAL(canceled()), this, SLOT(on_saveAllDirtyImagesCanceled()));
        this->progress_dialog_calc_filter->exec();
    }

    void FilterWindow::on_btn_save_all_dirtyimg_clicked()
    {
        QFileDialog dialog(this, tr("Select folder to save files"));
        uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptSave);
        if (dialog.exec() == QDialog::Accepted)
        {
            this->saveAllDirtyImages(dialog.selectedFiles().first());
        }
    }

    void FilterWindow::on_noise_selection_group_currentChanged(int index)
    {
        this->selected_noise_type = static_cast<AppLogic::noise_type_t>(index);
        this->selected_noise_value = this->getSelectedNoiseValue(this->selected_noise_type);
        this->setDirtyImage();
    }

    void FilterWindow::on_btn_load_single_image_clicked()
    {
        QFileDialog dialog(this, tr("Open File"));
        uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen, uihelp::file_dialog_type_t::FD_IMAGE, QFileDialog::FileMode::ExistingFiles);
        while (dialog.exec() == QDialog::Accepted && !loadImagesFromFile(dialog.selectedFiles()))
        {
        }
    }

    void FilterWindow::createFilterInfoDialog()
    {
        QMessageBox filter_info_dialog;
        FilterInfo *fi = this->filter_logic.getFilterInfo();
        filter_info_dialog.setText(tr("The filter calculation finised."));
        QString filter_info_message = QString("Filter name: %1\n"
                                              "Image Source: %2\n"
                                              "Calculation method: %3\n"
                                              "Size: %4\n"
                                              "Ammount of images: %5\n"
                                              "Noise type used: %6\n"
                                              "Noise value: %7\n"
                                              "Calculation time: %8")
                                          .arg(fi->filter_name.c_str())
                                          .arg(fi->file_image_source.c_str())
                                          .arg(fi->calculation_method.c_str())
                                          .arg(uihelp::generateSizeMessage(fi->f_matrix_num_row, fi->f_matrix_num_col))
                                          .arg(fi->image_calc_amount)
                                          .arg(fi->noise_type.c_str())
                                          .arg(fi->noise_value)
                                          .arg(QString("%1 s").arg(QString::number(fi->calc_time_seconds, 'f', 2)));

        filter_info_dialog.setInformativeText(filter_info_message);

        QPushButton *continueButton = filter_info_dialog.addButton(tr("Continue making filters"), QMessageBox::ActionRole);
        QPushButton *goToFiltersButton = filter_info_dialog.addButton(tr("Go to filter images"), QMessageBox::ButtonRole::RejectRole);

        filter_info_dialog.exec();

        if (filter_info_dialog.clickedButton() == continueButton)
        {
        }
        else if (filter_info_dialog.clickedButton() == goToFiltersButton)
        {
            this->on_actionFilter_image_triggered();
        }
    }

    void FilterWindow::on_lineEdit_filter_loaded_images_textChanged(const QString &arg1)
    {
        uihelp::filterListItems(ui->listWidget_loaded_images, arg1);
    }

    static const char notAllowedChars[] = "/\\,^@={}[]~!?:&*\"|#%<>$\"'();`' ";
    static const char *notAllowedSubStrings[] = {".."};

    bool FilterWindow::validateNameInput()
    {
        QString filter_name = ui->filter_name_input->text();
        if (filter_name.isEmpty())
        {
            this->setFilterNameInputError(tr("The filter name cannot be empty"));
            return false;
        }
        // Characters
        for (const char *c = notAllowedChars; *c; ++c)
            if (filter_name.contains(QLatin1Char(*c)))
            {
                this->setFilterNameInputError(tr("The filter name contains the illegal character: %1").arg(*c));
                return false;
            }
        // Substrings
        const int notAllowedSubStringCount = sizeof(notAllowedSubStrings) / sizeof(const char *);
        for (int s = 0; s < notAllowedSubStringCount; s++)
        {
            const QLatin1String notAllowedSubString(notAllowedSubStrings[s]);
            if (filter_name.contains(notAllowedSubString))
            {
                this->setFilterNameInputError(tr("The filter name contains the illegal substring: %1").arg(notAllowedSubString));
                return false;
            }
        }
        // Windows devices
        static const QRegExp rc(QLatin1String(WINDOWS_DEVICES), Qt::CaseInsensitive);
        if (rc.exactMatch(filter_name))
        {
            this->setFilterNameInputError(tr("The filter name contains a windows device name"));
            return false;
        }
        if (AppLogic::ImageLoader::existsNameFilter(filter_name.toStdString()))
        {
            this->setFilterNameInputError(tr("The filename already exists"));
            return false;
        }

        ui->filter_name_input->setStyleSheet("color:black; background-color: white");
        statusBar()->clearMessage();
        return true;
    }

    void FilterWindow::on_filter_name_input_editingFinished()
    {
        this->validateNameInput();
    }

    void FilterWindow::setFilterNameInputError(const QString &error_message)
    {
        statusBar()->showMessage(error_message, 3000);
        ui->filter_name_input->setFocus();
        ui->filter_name_input->setStyleSheet("color:black; background-color: red");
    }

    void FilterWindow::on_actionLoadImageFromZip_triggered()
    {
        QFileDialog dialog(this, tr("Open Zip file with images"));
        uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen, uihelp::file_dialog_type_t::FD_ZIP, QFileDialog::FileMode::ExistingFiles);
        if (dialog.exec() == QDialog::Accepted)
        {
            this->loadImagesFromZip(dialog.selectedFiles().first());
        }
    }

}