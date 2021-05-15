#include "Filter.hpp"

namespace AppLogic
{

    Filter::Filter()
    {
    }

    vector<string> Filter::loadImagesFromFolder(string folder_path)
    {
        this->working_images.clear();
        ImageLoader il;

        this->working_images = il.loadImagesFromFolder(folder_path.c_str());
        this->calc_info.file_image_source= il.getSourceDirectory();
        this->image_set = this->getWorkingImagesMat(); // can be done on a different thread to avoid blocking
        return il.getLoadedImagePaths();
    }

    vector<string> Filter::loadSingleImage(string folder_path)
    {
        ImageLoader il;

        this->working_images.clear();
        this->working_images.push_back(il.loadSingleImage(folder_path.c_str()));

        this->image_set = this->getWorkingImagesMat(); // can be done on a different thread to avoid blocking
        return il.getLoadedImageNames();
    }

    bool Filter::calculateFilter(size_t rank, calc_method_t calc_method)
    {
        // mat dirty_training_set = this->getWorkingImagesMat();
        // TO DO: Measure times and construct FilterInfo to return
        switch (calc_method)
        {
        case calc_method_t::RCIMA_METHOD:
            this->F_matrix = frcima::rcima(this->image_set, this->getWorkingImagesMat(), rank);
            break;
        case calc_method_t::FAST_RCIMA_METHOD:
            this->F_matrix = frcima::fast_rcima(this->image_set, this->getWorkingImagesMat(), rank);
            break;
        default:
            break;
        }
        this->calc_info.calc_time_seconds= 1; // calculation time

        this->calc_info.f_matrix_num_col =this->F_matrix.n_cols;
        this->calc_info.f_matrix_num_row = this->F_matrix.n_rows;
        this->calc_info.image_calc_amount= this->working_images.size();
        this->calc_info.rank = rank;        
        this->calc_info.calculation_method=calc_method;

        return true;
    }

    VecImage Filter::applyNoiseToImage(size_t image_id, double noise_value, noise_type_t noise_type)
    {
        VecImage result;
        if (image_id > this->working_images.size())
        {
            return result;
        }

        result = VecImage(this->working_images[image_id]);
        result.applyNoise(noise_value, noise_type);
        return result;
    }

    void Filter::applyNoiseToWorkingImages(double noise_value, noise_type_t noise_type )
    {

        for (auto &image : this->working_images)
            image.applyNoise(noise_value, noise_type);
        
        this->calc_info.noise_type= noise_type;
        this->calc_info.noise_value= noise_value;
    }

    bool Filter::saveToFile(string folder_path )
    {
        if (this->F_matrix.n_rows > 0)
        {
            string file_name = folder_path + this->calc_info.filter_name + FILTER_FILE_EXTENSION;
            return this->F_matrix.save(file_name);
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


    bool Filter::saveFilterInfo(string folder_path ){
        string file_name = folder_path + this->calc_info.filter_name + FILTER_INFO_EXTENSION;
        fstream fi_binary_file(file_name, std::ios::out | std::ios::binary | std::ios::app);
        if (fi_binary_file.is_open())
        {
            fi_binary_file << this->calc_info;
            fi_binary_file.close();
            return true;
        }

        return false;
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
