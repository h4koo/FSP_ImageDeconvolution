#include "imagedeconvolutionwindow.h"
#include "ui_imagedeconvolutionwindow.h"

using std::vector;
using std::string;
using std::cout; using std::endl;

ImageDeconvolutionWindow::ImageDeconvolutionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageDeconvolutionWindow),label_loaded_image(new QLabel),label_filtered_image(new QLabel)
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
    
    //get saved filters
     vector<string> filter_files = this->img_cleaning.getFilterNames();

    for(string filter : filter_files){
        ui->comboBox_filter_select->addItem(filter.c_str());
    }
}

ImageDeconvolutionWindow::~ImageDeconvolutionWindow()
{
    delete ui;
}

void ImageDeconvolutionWindow::on_next_loaded_image_btn_clicked()
{
    int current_row =ui->listWidget_loaded_images->currentRow();

    cout << "current row is " << current_row <<endl;
    cout << "list count is " << ui->listWidget_loaded_images->count() <<endl;
    if(current_row+1 < ui->listWidget_loaded_images->count()){
        ui->listWidget_loaded_images->setCurrentRow(current_row+1);
        cout << "new row is " << current_row <<endl;
    }
}

void ImageDeconvolutionWindow::on_previous_loaded_image_btn_clicked()
{
    int current_row =ui->listWidget_loaded_images->currentRow();

    cout << "current row is " << current_row <<endl;
    cout << "list count is " << ui->listWidget_loaded_images->count() <<endl;
    if(current_row-1 >= 0){
        ui->listWidget_loaded_images->setCurrentRow(current_row-1);
    }
}

bool ImageDeconvolutionWindow::loadFolder(const QString &fileName)
{
//    this->setLoadedImage(fileName);

//    setWindowFilePath(fileName);

    // Load folder from filter


    // add items to the list
    vector<string> string_of_text = this->img_cleaning.loadImagesFromFolder(fileName.toStdString());


    if (string_of_text.empty()) {
        const QString message = tr("Unable to load images at \"%1\"")
            .arg(QDir::toNativeSeparators(fileName));
        statusBar()->showMessage(message);
        return false;
    }

    ui->listWidget_loaded_images->clear();

    for(string item : string_of_text){
        ui->listWidget_loaded_images->addItem(QString(item.c_str()));
    }

    this->setLoadedImage(string_of_text[0].c_str());

    const QString message = tr("Opened folder: \"%1\", with image dimensions %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}


bool ImageDeconvolutionWindow::setLoadedImage(const QString &fileName){

    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {

        this->label_loaded_image->clear();

//        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
//                                 tr("Cannot load %1: %2")
//                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    this->image = newImage;
    this->label_loaded_image->setPixmap(QPixmap::fromImage(newImage));
    this->label_loaded_image->adjustSize();



    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;


}

void ImageDeconvolutionWindow::on_listWidget_loaded_images_currentRowChanged(int currentRow)
{
this->setLoadedImage( ui->listWidget_loaded_images->item(currentRow)->text());
// this->setFilteredImage(ui->listWidget_loaded_images->item(currentRow)->text());
}

void ImageDeconvolutionWindow::on_load_images_btn_clicked()
{
    QFileDialog dialog(this, tr("Open File"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFolder(dialog.selectedFiles().first())) {}
}

void ImageDeconvolutionWindow::on_listWidget_loaded_images_itemClicked(QListWidgetItem *item)
{
    this->setLoadedImage(item->text());
    // this->setFilteredImage(item->text());
}

void ImageDeconvolutionWindow::on_comboBox_filter_select_currentIndexChanged(int index)
{
    // get FilterInfo for selected filter

    //load selected filter
    if(!this->img_cleaning.loadFilter(index)) return ;

    AppLogic::FilterInfo * fi = this->img_cleaning.getFilterInfo(index);
    //set the vales for the labels
    ui->label_rank_value->setText(QString(fi->rank));
    ui->label_calc_method->setText(QString(fi->calculation_method));
    ui->label_f_mat_size->setText(QString(fi->f_matrix_num_row*fi->f_matrix_num_col));
    ui->label_source_image_folder->setText(QString(fi->file_image_source.c_str()));

    return;
}

// void ImageDeconvolutionWindow::on_pushButton_clicked()
// {
//     this->setFilteredImage(ui->listWidget_loaded_images->currentItem()->text());
// }

bool ImageDeconvolutionWindow::setFilteredImage(const QString &fileName){

    

    // Applyfilter to current image
    AppLogic::VecImage vi = this->img_cleaning.applyFilterToImage(ui->listWidget_loaded_images->currentRow());
    // construct filtered image

    const QImage f_image(vi.getRawImageData(), vi.num_cols(), vi.num_rows(),vi.num_cols(), QImage::Format::Format_Grayscale8);
    
    if (f_image.isNull()) {

        this->label_filtered_image->clear();

//        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
//                                 tr("Cannot load %1: %2")
//                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    this->filtered_image = f_image;
    this->label_filtered_image->setPixmap(QPixmap::fromImage(this->filtered_image));
    this->label_filtered_image->adjustSize();

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(filtered_image.width()).arg(filtered_image.height()).arg(filtered_image.depth());
    statusBar()->showMessage(message);
    return true;
}


void ImageDeconvolutionWindow::on_pushButton_filter_image_clicked()
{
    this->setFilteredImage(ui->listWidget_loaded_images->currentItem()->text());
}