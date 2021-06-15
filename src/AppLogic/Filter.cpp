#include "Filter.hpp"


namespace AppLogic
{

    Filter::Filter() : last_used_noise_type(noise_type_t::GAUSSIAN), last_used_noise_value(0), is_canceled(false)
    {
    }

    string Filter::getImagePath(size_t index)
    {
        if (index > this->loaded_file_paths.size())
            index = 0;
        return this->loaded_file_paths[index];
    }
    vector<string> Filter::loadImagesFromFolder(string folder_path)
    {
        this->clearWorkingImages();
        ImageLoader il;
        this->working_images = il.loadImagesFromFolder(folder_path.c_str());
        this->loaded_file_names = il.getLoadedImageNames();
        this->loaded_file_paths = il.getLoadedImagePaths();
        this->calc_info.file_image_source = il.getSourceDirectory();

        this->image_set = this->getWorkingImagesMat();
        return this->loaded_file_names;
    }

    string Filter::loadSingleImage(string folder_path)
    {
        ImageLoader il;

        // this->working_images.clear();
        this->working_images.push_back(il.loadSingleImage(folder_path.c_str()));
        this->loaded_file_names.push_back(il.getLoadedImageNames()[0]);
        this->loaded_file_paths.push_back(il.getLoadedImagePaths()[0]);

        this->image_set = this->getWorkingImagesMat();
        return il.getLoadedImageNames()[0];
    }

    bool Filter::calculateFilter(size_t rank, calc_method_t calc_method)
    {
        try{
        std::chrono::_V2::system_clock::time_point t1, t2;
        switch (calc_method)
        {
        case calc_method_t::RCIMA_METHOD:
            t1 = high_resolution_clock::now();
            this->F_matrix = frcima::rcima(this->image_set, this->getDirtyWorkingImagesMat(), rank);
            t2 = high_resolution_clock::now();
            break;
        case calc_method_t::FAST_RCIMA_METHOD:
            t1 = high_resolution_clock::now();
            this->F_matrix = frcima::fast_rcima(this->image_set, this->getDirtyWorkingImagesMat(), rank);
            t2 = high_resolution_clock::now();
            break;
        default:
            break;
        }
        auto duration = duration_cast<std::chrono::milliseconds>(t2 - t1);
        this->calc_info.calc_time_seconds = float(duration.count()) / 1000;
        this->calc_info.f_matrix_num_col = this->F_matrix.n_cols;
        this->calc_info.f_matrix_num_row = this->F_matrix.n_rows;
        this->calc_info.image_calc_amount = this->working_images.size();
        this->calc_info.rank = rank;
        this->calc_info.calculation_method = Filter::getCalculationMethodName(calc_method);
        this->calc_info.noise_type = VecImage::getNoiseName(this->last_used_noise_type);
        this->calc_info.noise_value = this->last_used_noise_value;

        return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }        
    }

    VecImage Filter::applyNoiseToImage(size_t image_id, size_t noise_value, noise_type_t noise_type)
    {
        this->last_used_noise_type = noise_type;
        this->last_used_noise_value = noise_value;
        VecImage result;
        if (image_id > this->working_images.size())
        {
            return result;
        }
        result = VecImage(this->working_images[image_id]);
        result.applyNoise(noise_value, noise_type);
        return result;
    }

    // void Filter::applyNoiseToWorkingImages(size_t noise_value, noise_type_t noise_type)
    // {
    //     this->last_used_noise_type = noise_type;
    //     this->last_used_noise_value = noise_value;

    //     for (auto &image : this->working_images)
    //         image.applyNoise(noise_value, noise_type);

    //     this->calc_info.noise_type = VecImage::getNoiseName(noise_type);
    //     this->calc_info.noise_value = noise_value;
    // }

    bool Filter::saveToFile(string folder_path)
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
            size_t rows = working_images[0].numCols() * working_images[0].numRows(); // training set size of vectorized image

            w_image_set = mat(rows, columns);
            for (size_t col = 0; col < columns; ++col)
            {
                vec current_image = this->working_images[col].getDoubleData();
                for (size_t r = 0; r < rows; ++r)
                {
                    w_image_set(r, col) = current_image(r);
                }
            }
        }
        return w_image_set;
    }

    mat Filter::getDirtyWorkingImagesMat()
    {
        mat w_image_set;

        if (this->working_images.size())
        {
            size_t columns = working_images.size();                                    //training set amount of images
            size_t rows = working_images[0].numCols() * working_images[0].numRows(); // training set size of vectorized image

            w_image_set = mat(rows, columns);
            for (size_t col = 0; col < columns; ++col)
            {   
                VecImage v_img(this->working_images[col]);
                v_img.applyNoise(this->last_used_noise_value, this->last_used_noise_type);
                vec current_image = v_img.getVecDoubleData();
                for (size_t r = 0; r < rows; ++r)
                {
                    w_image_set(r, col) = current_image(r);
                }
            }
        }
        return w_image_set;
    }

    bool Filter::saveFilterInfo(string folder_path)
    {
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

    void Filter::cancelSaveDirtyImages()
    {
        this->is_canceled = true;
    }

    bool Filter::saveAllDirtyImages(string folder_path)
    {

        for (size_t index = 0; index < this->working_images.size(); ++index)
        {
            if (this->is_canceled)
            {
                this->is_canceled = false;
                return false;
            }
            string filename = folder_path + "/" + this->loaded_file_names[index] + DIRTY_IMAGE_SUFFIX;
            if (!this->applyNoiseToImage(index, this->last_used_noise_value, this->last_used_noise_type).save(filename))
                return false;
        }

        return true;
    }

    bool Filter::saveDirtyImage(size_t image_id, string folder_path)
    {
        if (image_id > this->working_images.size())
            return false;
        return this->applyNoiseToImage(image_id, this->last_used_noise_value, this->last_used_noise_type).save(folder_path);
    }

    string Filter::getCalculationMethodName(calc_method_t calc_method)
    {
        // **NOTE** don't add whitespace to the names since it breaks FilterInfo loading
        switch (calc_method)
        {
        case calc_method_t::FAST_RCIMA_METHOD:
            return "Fast-RCIMA";
        case calc_method_t::RCIMA_METHOD:
            return "RCIMA";
        default:
            return "Unknown";
        }
    }

    vector<string> Filter::loadImagesFromZip(string file_path)
    {
        this->clearWorkingImages();
        ImageLoader il;
        this->working_images = il.loadImagesFromZip(file_path.c_str());
        this->loaded_file_names = il.getLoadedImageNames();
        this->loaded_file_paths = il.getLoadedImagePaths();
        this->calc_info.file_image_source = il.getSourceDirectory();

        this->image_set = this->getWorkingImagesMat(); // can be done on a different thread to avoid blocking
        return this->loaded_file_names;
    }

    VecImage *Filter::getLoadedImage(const size_t index)
    {
        if (index > this->working_images.size())
            return NULL;
        return &(this->working_images[index]);
    }
}

