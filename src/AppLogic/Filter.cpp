#include "Filter.hpp"

namespace AppLogic
{

    Filter::Filter()
    {
    }

    vector<string> Filter::loadImagesFromFolder(string folder_path)
    {
        this->working_images.clear();
        // this->loaded
        ImageLoader il;

        this->working_images = il.loadImagesFromFolder(folder_path.c_str());
        this->image_set = this->getWorkingImagesMat(); // can be done on a different thread to avoid blocking
        return il.getLoadedImageNames();
    }
    vector<string> Filter::loadSingleImage(string folder_path)
    {
        ImageLoader il;

        this->working_images.clear();
        this->working_images.push_back(il.loadSingleImage(folder_path.c_str()));

        this->image_set = this->getWorkingImagesMat(); // can be done on a different thread to avoid blocking
        return il.getLoadedImageNames();
    }
    bool Filter::calculateFilter(size_t rank)
    {
        // mat dirty_training_set = this->getWorkingImagesMat();

        this->F_matrix = frcima::rcima(this->image_set, this->getWorkingImagesMat(), rank);
        return true;
    }

    void Filter::applyNoiseToWorkingImages()
    {

        for (auto &image : this->working_images)
            image.applyNoise();
    }

    bool Filter::saveToFile(string file_path)
    {
        if (this->F_matrix.n_rows > 0)
        {
            return this->F_matrix.save(file_path);
        }
        return false;
    }

    bool Filter::loadFmatrixFromFile(string file_path)
    {
        this->F_matrix.reset();
        return this->F_matrix.load(file_path);
    }

    mat Filter::getWorkingImagesMat()
    {
        mat w_image_set;

        if (this->working_images.size())
        {
            size_t columns = working_images.size();                                    //training set amount of images
            size_t rows = working_images[0].num_cols() * working_images[0].num_rows(); // training set size of vectorized image

            w_image_set = mat(rows, columns);

            // for (VecImage current_image : this->working_images){

            // }
            for (size_t col = 0; col < columns; ++col)
            {
                vec current_image = this->working_images[col].get_double_data();
                for (size_t r = 0; r < rows; ++r)
                {
                    w_image_set(r, col) = current_image(r);
                }
            }
        }
        return w_image_set;
    }

}

// +applyNoiseToImage(image_id : int, noise_type, value : double) : VecImage
// +applyNoiseToWorkingImages() : VecImage[]
// +loadImagesFromFolder(folder_path : string) : string[]
// +loadImagesFromZip(file_path : string) : string[]
// +getNoisyImage(image_id : int)
// +calculateFilter(rank : unsigned int, calc_method : enum)
// +saveToFile(name : string, file_path : string)
// +saveAllDirtyImages(folder_path : string) : bool
// +saveDirtyImage(image_id : int, folder_path : string) : bool
// -saveFilterInfo()
// +exportFilter() : bool
// +importFilter(file_path : string) : bool

// -calc_info : FilterInfo
// -working_images : VecImage[]
// -working_noise : Noise
// -image_set :double[][]
// -f_matrix : double[][]
