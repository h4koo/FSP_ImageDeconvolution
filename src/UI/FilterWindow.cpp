#include "filterwindow.h"
#include "ui_filterwindow.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

FilterWindow::FilterWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FilterWindow), label_loaded_image(new QLabel), label_dirty_image(new QLabel)
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
}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::on_load_images_btn_clicked()
{
    QFileDialog dialog(this, tr("Open File"));
    uihelp::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFolder(dialog.selectedFiles().first()))
    {
    }
}

bool FilterWindow::loadFolder(const QString &fileName)
{
    //    this->setLoadedImage(fileName);

    //    setWindowFilePath(fileName);

    // Load folder from filter

    // add items to the list
    vector<string> string_of_text = this->filter_logic.loadImagesFromFolder(fileName.toStdString());

    if (string_of_text.empty())
    {
        const QString message = tr("Unable to load images at \"%1\"")
                                    .arg(QDir::toNativeSeparators(fileName));
        statusBar()->showMessage(message);
        return false;
    }

    ui->listWidget_loaded_images->clear();

    for (string item : string_of_text)
    {
        ui->listWidget_loaded_images->addItem(QString(item.c_str()));
    }

    this->setLoadedImage(string_of_text[0].c_str());

    unsigned int img_size = this->image.height() * this->image.width();
    unsigned int max_rank = string_of_text.size() < img_size ? string_of_text.size() : img_size;

    ui->rank_value_spinBox->setRange(1, max_rank);
    ui->filter_name_input->setText("filter_");

    ui->label_min_max_rank->setText(QString("min: 1  max: %1").arg(max_rank));

    const QString message = tr("Opened folder: \"%1\", with image dimensions %2x%3, Depth: %4")
                                .arg(QDir::toNativeSeparators(fileName))
                                .arg(image.width())
                                .arg(image.height())
                                .arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}

bool FilterWindow::setLoadedImage(const QString &fileName)
{

    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull())
    {

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
                                .arg(QDir::toNativeSeparators(fileName))
                                .arg(image.width())
                                .arg(image.height())
                                .arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}

bool FilterWindow::setDirtyImage(const QString &fileName)
{

    double noise_value = 0;
    AppLogic::noise_type_t noise_type = AppLogic::noise_type_t::GAUSSIAN;

    if (ui->gauss_noise_tab->isVisible())
    {
        cout << "gauss noise tab enabled" << endl;
        noise_type = AppLogic::noise_type_t::GAUSSIAN;
        noise_value = ui->gaussian_value->text().toInt();
    }
    if (ui->snp_noise_tab->isVisible())
    {
        cout << "snp noise tab enabled" << endl;
        noise_type = AppLogic::noise_type_t::SALT_PEPPER;
        //        noise_value = ui->sn->text().toInt();
    }

    cout << "noise value: " << noise_value << endl;
    // apply noise to currently selected image
    //applyNoiseToImage(ui->listWidget_loaded_images->currentRow(), noise_type, value)
    AppLogic::VecImage vi = this->filter_logic.applyNoiseToImage(ui->listWidget_loaded_images->currentRow(), noise_type, noise_value);

    //   ui->listWidget_loaded_images->currentRow();

    // filter apply noise (

    // get dirty image

    const QImage d_image(vi.getRawImageData(), vi.num_cols(), vi.num_rows(), vi.num_cols(), QImage::Format::Format_Grayscale8);

    if (d_image.isNull())
    {

        this->label_dirty_image->clear();

        //        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
        //                                 tr("Cannot load %1: %2")
        //                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    this->dirty_image = d_image;
    this->label_dirty_image->setPixmap(QPixmap::fromImage(this->dirty_image));
    this->label_dirty_image->adjustSize();

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
                                .arg(QDir::toNativeSeparators(fileName))
                                .arg(dirty_image.width())
                                .arg(dirty_image.height())
                                .arg(dirty_image.depth());
    statusBar()->showMessage(message);
    return true;
}

void FilterWindow::on_listWidget_loaded_images_itemClicked(QListWidgetItem *item)
{
    this->setLoadedImage(item->text());
    this->setDirtyImage("");
}

void FilterWindow::on_next_loaded_image_btn_clicked()
{

    int current_row = ui->listWidget_loaded_images->currentRow();

    cout << "current row is " << current_row << endl;
    cout << "list count is " << ui->listWidget_loaded_images->count() << endl;
    if (current_row + 1 < ui->listWidget_loaded_images->count())
    {
        ui->listWidget_loaded_images->setCurrentRow(current_row + 1);
        cout << "new row is " << current_row << endl;
    }
}

void FilterWindow::on_listWidget_loaded_images_itemEntered(QListWidgetItem *item)
{
    this->setLoadedImage(item->text());
    this->setDirtyImage("");
}

void FilterWindow::on_listWidget_loaded_images_currentRowChanged(int currentRow)
{
    this->setLoadedImage(ui->listWidget_loaded_images->item(currentRow)->text());
    this->setDirtyImage("");
}

void FilterWindow::on_previous_loaded_image_btn_clicked()
{
    int current_row = ui->listWidget_loaded_images->currentRow();

    cout << "current row is " << current_row << endl;
    cout << "list count is " << ui->listWidget_loaded_images->count() << endl;
    if (current_row - 1 >= 0)
    {
        ui->listWidget_loaded_images->setCurrentRow(current_row - 1);
    }
}

void FilterWindow::on_calc_filter_btn_clicked()
{
    //check if the name already exists
    // to do
    string filter_name = ui->filter_name_input->text().toStdString(); //name of filter

    cout<< "El nombre del filtro es"<<filter_name << endl;
    // Get Noise value
    double noise_value = 0;
    AppLogic::noise_type_t noise_type = AppLogic::noise_type_t::GAUSSIAN;

    if (ui->gauss_noise_tab->isVisible())
    {
        cout << "gauss noise tab enabled" << endl;
        noise_value = ui->gaussian_value->text().toDouble();
        noise_type = AppLogic::noise_type_t::GAUSSIAN;
    }
    if (ui->snp_noise_tab->isVisible())
    {
        cout << "snp noise tab enabled" << endl;
        //        noise_value = ui->sn->text().toInt();
        noise_type = AppLogic::noise_type_t::SALT_PEPPER;
    }

    cout << "noise value: " << noise_value << endl;

    //Apply Noise to all images

    // applyNoiseToAllImages(noise_type, noise_value);

    this->filter_logic.setNoise(noise_type);
    this->filter_logic.setNoiseValue(noise_value);

    int rank_val = ui->rank_value_spinBox->value(); // rank value

    //check the selected method
    AppLogic::calc_method_t selected_method = AppLogic::calc_method_t::RCIMA_METHOD;
    if (ui->rcima_rbtn->isChecked())
        selected_method = AppLogic::calc_method_t::RCIMA_METHOD;
    else
    {
        selected_method = AppLogic::calc_method_t::FAST_RCIMA_METHOD;
    }
    // ui->fastrcima_rbtn->isChecked();

    //calculate filter
    if(!this->filter_logic.createFilter(filter_name, rank_val, selected_method)){
        cout<< "No se pudo crear el filtro" <<endl;
    }


}


void FilterWindow::on_gaussian_slider_sliderReleased()
{
    this->setDirtyImage("");
}