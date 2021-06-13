#include "imagedeconvolutionwindow.h"
#include "ui_imagedeconvolutionwindow.h"

#define ELIDE_WIDTH 400

using std::cout;
using std::endl;
using std::string;
using std::vector;

ImageDeconvolutionWindow::ImageDeconvolutionWindow(QWidget *parent) : QMainWindow(parent),
                                                                      ui(new Ui::ImageDeconvolutionWindow), label_loaded_image(new QLabel), label_filtered_image(new QLabel), is_filter_loaded(false)
{
    ui->setupUi(this);
    //create label to load image
    this->label_loaded_image->setBackgroundRole(QPalette::Base);
    this->label_loaded_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->label_loaded_image->setScaledContents(true);

    //create label to show dirty/filtered image
    this->label_filtered_image->setBackgroundRole(QPalette::Base);
    this->label_filtered_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->label_filtered_image->setScaledContents(true);

    //set up the label inside the scroll area
    ui->loaded_image_area->setBackgroundRole(QPalette::Dark);
    ui->loaded_image_area->setWidget(this->label_loaded_image);

    //set up the label inside the scroll area
    ui->filtered_image_area->setBackgroundRole(QPalette::Dark);
    ui->filtered_image_area->setWidget(this->label_filtered_image);

    //load the filter list
    this->updateFilterList();
}

ImageDeconvolutionWindow::~ImageDeconvolutionWindow()
{
    delete ui;
}

void ImageDeconvolutionWindow::updateFilterList()
{
    ui->comboBox_filter_select->clear();
    vector<string> filter_files = this->img_cleaning.getFilterNames();
    for (string filter : filter_files)
    {
        ui->comboBox_filter_select->addItem(filter.c_str());
    }
}

void ImageDeconvolutionWindow::on_filterLoadingFinished()
{
    if (!this->loadfilter_future_watcher.result())
    {
        statusBar()->showMessage("Unable to load filter", LONGER_MESSAGE_TIME);
        return;
    }
    statusBar()->showMessage("Finished loading filter", STANDARD_MESSAGE_TIME);
    this->is_filter_loaded = true;
}

void ImageDeconvolutionWindow::on_next_loaded_image_btn_clicked()
{
    int current_row = ui->listWidget_loaded_images->currentRow();
    if (current_row < 0)
        return;
    if (current_row + 1 < ui->listWidget_loaded_images->count())
        ui->listWidget_loaded_images->setCurrentRow(current_row + 1);
}

void ImageDeconvolutionWindow::on_previous_loaded_image_btn_clicked()
{
    int current_row = ui->listWidget_loaded_images->currentRow();
    if (current_row < 0)
        return;
    if (current_row - 1 >= 0)
        ui->listWidget_loaded_images->setCurrentRow(current_row - 1);
}

void ImageDeconvolutionWindow::on_loadImagesFinished()
{
    this->progress_dialog_filter->setMaximum(100);
    this->progress_dialog_filter->setValue(100);

    vector<string> loaded_filenames = this->future_watcher_load_images.result();
    if (loaded_filenames.empty())
    {
        statusBar()->showMessage(tr("Unable to load images"), LONGER_MESSAGE_TIME);
        return;
    }

    statusBar()->showMessage(tr("Loaded %1 images").arg(loaded_filenames.size()), LONGER_MESSAGE_TIME);
    this->loadImageList(loaded_filenames);
}

void ImageDeconvolutionWindow::on_loadImagesCanceled()
{
    AppLogic::ImageLoader::cancelOperation(true);
    statusBar()->showMessage(tr("Load images canceled"), LONGER_MESSAGE_TIME);
}

void ImageDeconvolutionWindow::loadImagesFromZip(const QString &folder_path)
{

    QFuture<vector<string>> image_loading = QtConcurrent::run(&this->img_cleaning, &AppLogic::ImageCleaningLogic::loadImagesFromZip, folder_path.toStdString());
    this->future_watcher_load_images.setFuture(image_loading);
    connect(&this->future_watcher_load_images, SIGNAL(finished()), this, SLOT(on_loadImagesFinished()));

    //create a progress dialog
    this->progress_dialog_filter = new QProgressDialog(tr("Loading Images"), tr("Stop"), 0, 0, this);
    this->progress_dialog_filter->setWindowModality(Qt::WindowModal);
    connect(this->progress_dialog_filter, SIGNAL(canceled()), this, SLOT(on_loadImagesCanceled()));
    this->progress_dialog_filter->exec();
}

void ImageDeconvolutionWindow::loadImagesFromFolder(const QString &folder_path)
{
    // load images in a thread
    QFuture<vector<string>> image_loading = QtConcurrent::run(&this->img_cleaning, &AppLogic::ImageCleaningLogic::loadImagesFromFolder, folder_path.toStdString());
    this->future_watcher_load_images.setFuture(image_loading);
    connect(&this->future_watcher_load_images, SIGNAL(finished()), this, SLOT(on_loadImagesFinished()));

    //create a progress dialog
    this->progress_dialog_filter = new QProgressDialog(tr("Loading Images"), tr("Stop"), 0, 0, this);
    this->progress_dialog_filter->setWindowModality(Qt::WindowModal);
    connect(this->progress_dialog_filter, SIGNAL(canceled()), this, SLOT(on_loadImagesCanceled()));
    this->progress_dialog_filter->exec();
}

bool ImageDeconvolutionWindow::loadImagesFromFile(const QStringList &filenames)
{

    this->img_cleaning.clearWorkingImages();
    vector<string> loaded_filenames;
    for (auto &&filename : filenames)
    {
        loaded_filenames.push_back(this->img_cleaning.addImageToWorkingImages(filename.toStdString()));
    }

    if (loaded_filenames.empty())
    {
        const QString message = tr("Unable to load images at \"%1\"")
                                    .arg(QDir::toNativeSeparators(filenames[0]));
        statusBar()->showMessage(message);
    }
    return loadImageList(loaded_filenames);
}

bool ImageDeconvolutionWindow::loadImageList(const vector<string> &loaded_filenames)
{
    ui->listWidget_loaded_images->clear();
    for (string item : loaded_filenames)
    {
        ui->listWidget_loaded_images->addItem(QString(item.c_str()));
    }

    ui->listWidget_loaded_images->setCurrentRow(0);
    this->setLoadedImage(0);

    return true;
}

bool ImageDeconvolutionWindow::setLoadedImage(const size_t index)
{
    AppLogic::VecImage *vi = this->img_cleaning.getLoadedImage(index);
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

bool ImageDeconvolutionWindow::setFilteredImage(const size_t index)
{
    // Applyfilter to current image
    AppLogic::VecImage vi = this->img_cleaning.applyFilterToImage(index);
    // construct filtered image
    const QImage f_image(vi.getRawImageData(), vi.numCols(), vi.numRows(), vi.numCols(), QImage::Format::Format_Grayscale8);
    if (f_image.isNull())
    {
        this->label_filtered_image->clear();
        return false;
    }
    this->filtered_image = f_image;
    this->label_filtered_image->setPixmap(QPixmap::fromImage(this->filtered_image));
    this->label_filtered_image->adjustSize();
    return true;
}

void ImageDeconvolutionWindow::on_listWidget_loaded_images_currentRowChanged(int currentRow)
{
    this->setLoadedImage(currentRow);
    if (this->is_filter_loaded)
        this->setFilteredImage(currentRow);
}

void ImageDeconvolutionWindow::on_btn_load_image_clicked()
{

    QFileDialog dialog(this, tr("Select images to load"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen, uihelp::file_dialog_type_t::FD_IMAGE, QFileDialog::FileMode::ExistingFiles);

    while (dialog.exec() == QDialog::Accepted && !loadImagesFromFile(dialog.selectedFiles()))
    {
    }
}
void ImageDeconvolutionWindow::on_btn_load_folder_clicked()
{

    QFileDialog dialog(this, tr("Open folder"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    if (dialog.exec() == QDialog::Accepted)
    {
        this->loadImagesFromFolder(dialog.selectedFiles().first());
    }
}

void ImageDeconvolutionWindow::on_comboBox_filter_select_currentIndexChanged(int index)
{
    if (index < 0)
    {
        this->displayFilterInfo(0);
        return;
    }
    this->is_filter_loaded = false;
    AppLogic::FilterInfo *fi = this->img_cleaning.getFilterInfo(index);
    this->displayFilterInfo(fi);

    QFuture<bool> load_filter_result = QtConcurrent::run(&this->img_cleaning, &AppLogic::ImageCleaningLogic::loadFilter, index);
    this->loadfilter_future_watcher.setFuture(load_filter_result);
    connect(&this->loadfilter_future_watcher, SIGNAL(finished()), this, SLOT(on_filterLoadingFinished()));
    statusBar()->showMessage(tr("Loading filter matrix ..."));
}

void ImageDeconvolutionWindow::displayFilterInfo(AppLogic::FilterInfo *fi)
{

    if (fi != nullptr)
    {
        //font metric for elided text (the ... at the end)
        QFontMetricsF fm(QString(fi->file_image_source.c_str()));
        //set the values for the labels
        ui->label_rank_value->setText(QString::number(fi->rank));
        ui->label_calc_method->setText(QString(fi->calculation_method.c_str()));
        const QString time_message = tr("%1 s")
                                         .arg(QString::number(fi->calc_time_seconds, 'f', 2));
        ui->label_calctime->setText(time_message);
        const QString size_message = uihelp::generateSizeMessage(fi->f_matrix_num_row, fi->f_matrix_num_col);
        ui->label_f_mat_size->setText(size_message);
        ui->label_source_image_folder->setToolTip(QString(fi->file_image_source.c_str()));
        ui->label_source_image_folder->setText(fm.elidedText(QString(fi->file_image_source.c_str()), Qt::TextElideMode::ElideRight, ELIDE_WIDTH));
        ui->label_noisetype->setText(QString(fi->noise_type.c_str()));
        ui->label_noisevalue->setText(QString::number(fi->noise_value));
        ui->label_imageammount->setText(QString::number(fi->image_calc_amount));
    }
    else
    {
        ui->label_rank_value->setText("");
        ui->label_calc_method->setText("");
        ui->label_calctime->setText("");
        ui->label_f_mat_size->setText("");
        ui->label_source_image_folder->setToolTip("");
        ui->label_source_image_folder->setText("");
        ui->label_noisetype->setText("");
        ui->label_noisevalue->setText("");
        ui->label_imageammount->setText("");
    }
}

void ImageDeconvolutionWindow::on_pushButton_save_filterimage_clicked()
{
    QFileDialog dialog(this, tr("Save filtered image as"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptSave, uihelp::file_dialog_type_t::FD_IMAGE, QFileDialog::FileMode::AnyFile);
    while (dialog.exec() == QDialog::Accepted && !saveFilteredImage(dialog.selectedFiles().first()))
    {
    }
}

void ImageDeconvolutionWindow::on_pushButton_filterall_save_clicked()
{
    QFileDialog dialog(this, tr("Select folder to save filtered images"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptSave);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->saveAllFilteredImages(dialog.selectedFiles().first());
    }
}

void ImageDeconvolutionWindow::on_actionCreate_filter_triggered()
{
    FilterWindow *fw = new FilterWindow();
    fw->show();
    this->close();
}

bool ImageDeconvolutionWindow::saveFilteredImage(const QString &fileName)
{
    return this->img_cleaning.saveFilteredImage(ui->listWidget_loaded_images->currentRow(), fileName.toStdString());
}

void ImageDeconvolutionWindow::on_filterAllImagesCanceled()
{
    this->img_cleaning.cancelFilterAllImages();
}

void ImageDeconvolutionWindow::on_filterAllImagesFinished()
{
    this->progress_dialog_filter->setMaximum(100);
    this->progress_dialog_filter->setValue(100);
    if (this->bool_future_watcher.result())
    {
        statusBar()->showMessage(tr("Finished saving filtered images"), STANDARD_MESSAGE_TIME);
        return;
    }
    else
    {
        statusBar()->showMessage(tr("Error saving filtered images"), LONGER_MESSAGE_TIME);
    }
}

void ImageDeconvolutionWindow::saveAllFilteredImages(const QString &fileName)
{

    //save filtered images in a thread
    QFuture<bool> image_filtering = QtConcurrent::run(&this->img_cleaning, &AppLogic::ImageCleaningLogic::saveAllFilteredImages, fileName.toStdString());
    this->bool_future_watcher.setFuture(image_filtering);
    connect(&this->bool_future_watcher, SIGNAL(finished()), this, SLOT(on_filterAllImagesFinished()));

    //create a progress dialog
    this->progress_dialog_filter = new QProgressDialog(tr("Saving all filtered images"), tr("Stop"), 0, 0, this);
    this->progress_dialog_filter->setWindowModality(Qt::WindowModal);
    connect(this->progress_dialog_filter, SIGNAL (canceled()), this , SLOT(on_filterAllImagesCanceled()));
    this->progress_dialog_filter->exec();
}

void ImageDeconvolutionWindow::on_pushButton_deletefilter_clicked()
{
    size_t index = ui->comboBox_filter_select->currentIndex();
    if (this->img_cleaning.deleteFilter(index))
        ui->comboBox_filter_select->removeItem(index);
}

void ImageDeconvolutionWindow::on_pushButton_exportfilter_clicked()
{
    QFileDialog dialog(this, tr("Save exported filter as"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptSave, uihelp::file_dialog_type_t::FD_ZIP, QFileDialog::FileMode::AnyFile);

    if (dialog.exec() == QDialog::Accepted)
    {
        statusBar()->showMessage(tr("Exporting filter..."));
        this->exportFilter(dialog.selectedFiles().first());
    }
}

void ImageDeconvolutionWindow::on_pushButton_importfilter_clicked()
{

    QFileDialog dialog(this, tr("Open file to import"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen, uihelp::file_dialog_type_t::FD_ZIP, QFileDialog::FileMode::ExistingFile);
    if (dialog.exec() == QDialog::Accepted)
    {
        statusBar()->showMessage(tr("Importing filter..."));
        this->importFilter(dialog.selectedFiles().first());
    }
}

void ImageDeconvolutionWindow::on_lineEdit_filter_loaded_images_textChanged(const QString &arg1)
{
    uihelp::filterListItems(ui->listWidget_loaded_images, arg1);
}

void ImageDeconvolutionWindow::importFilter(const QString &import_filename)
{
    QFuture<bool> filter_import = QtConcurrent::run(&AppLogic::ImageLoader::importFilterFromZip, import_filename.toStdString());
    this->bool_future_watcher.setFuture(filter_import);
    connect(&this->bool_future_watcher, SIGNAL(finished()), this, SLOT(on_importFilterFinished()));

    //create a progress dialog
    this->progress_dialog_filter = new QProgressDialog(tr("Importing filter..."), tr("Stop"), 0, 0, this);
    this->progress_dialog_filter->setWindowModality(Qt::WindowModal);
    connect(this->progress_dialog_filter, SIGNAL(canceled()), this, SLOT(on_importFilterCanceled()));
    this->progress_dialog_filter->exec();
}

void ImageDeconvolutionWindow::exportFilter(const QString &export_filename)
{
    QFuture<bool> filter_export = QtConcurrent::run(&AppLogic::ImageLoader::createExportZip, export_filename.toStdString(), ui->comboBox_filter_select->currentText().toStdString());
    this->bool_future_watcher.setFuture(filter_export);
    connect(&this->bool_future_watcher, SIGNAL(finished()), this, SLOT(on_exportFilterFinished()));

    //create a progress dialog
    this->progress_dialog_filter = new QProgressDialog(tr("Exporting filter..."), tr("Stop"), 0, 0, this);
    this->progress_dialog_filter->setWindowModality(Qt::WindowModal);
    connect(this->progress_dialog_filter, SIGNAL(canceled()), this, SLOT(on_exportFilterCanceled()));
    this->progress_dialog_filter->exec();
}

void ImageDeconvolutionWindow::on_actionLoadImageFromZip_triggered()
{
    QFileDialog dialog(this, tr("Open Zip file with images"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen, uihelp::file_dialog_type_t::FD_ZIP, QFileDialog::FileMode::ExistingFile);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->loadImagesFromZip(dialog.selectedFiles().first());
    }
}

void ImageDeconvolutionWindow::on_exportFilterFinished()
{
    this->progress_dialog_filter->setMaximum(100);
    this->progress_dialog_filter->setValue(100);
    if (this->bool_future_watcher.result())
    {
        statusBar()->showMessage(tr("Finished exporting filter"), STANDARD_MESSAGE_TIME);
        return;
    }
    else
    {
        statusBar()->showMessage(tr("Error when exporting filter"), LONGER_MESSAGE_TIME);
    }
}

void ImageDeconvolutionWindow::on_exportFilterCanceled()
{
    AppLogic::ImageLoader::cancelOperation(true);
}

void ImageDeconvolutionWindow::on_importFilterFinished()
{
    this->progress_dialog_filter->setMaximum(100);
    this->progress_dialog_filter->setValue(100);

    if (this->bool_future_watcher.result())
    {
        this->updateFilterList();
        statusBar()->showMessage(tr("Finished importing filter"), STANDARD_MESSAGE_TIME);
        return;
    }
    else
    {

        statusBar()->showMessage(tr("Error importing filter"), LONGER_MESSAGE_TIME);
    }
}

void ImageDeconvolutionWindow::on_importFilterCanceled()
{
    AppLogic::ImageLoader::cancelOperation(true);
}