#include "FilterLogic.hpp"


namespace AppLogic
{

    FilterLogic::FilterLogic()
    {
        this->is_canceled = false;
    }

    vector<string> FilterLogic::loadImagesFromFolder(string folder_path)
    {

        return this->new_filter.loadImagesFromFolder(folder_path);
    }

    vector<string> FilterLogic::loadImagesFromZip(string file_path)
    {
        return this->new_filter.loadImagesFromZip(file_path);
    }

    VecImage *FilterLogic::getLoadedImage(const size_t index)
    {
        return this->new_filter.getLoadedImage(index);
    }

    VecImage FilterLogic::applyNoiseToImage(int image_id, noise_type_t noise_type, double noise_value)
    {
        this->last_noise = noise_type;
        this->last_noise_value = noise_value;
        return this->new_filter.applyNoiseToImage(image_id, noise_value, noise_type);
    }

    bool FilterLogic::createFilter(string name, int rank, calc_method_t calc_method)
    {

        if (ImageLoader::existsNameFilter(name))
        {
            std::cout << "the filename already exists" << std::endl;
            return false;
        }
        this->new_filter.applyNoiseToWorkingImages(this->last_noise_value, this->last_noise);
    
        std::thread thrd = std::thread(&Filter::calculateFilter, &this->new_filter, rank, calc_method);
        this->filter_calc_thread = thrd.native_handle();
        thrd.join();

        if (this->is_canceled)
        {
            this->is_canceled = false;
            return false;
        }

        this->new_filter.setFilterName(name);

        if (!this->new_filter.saveToFile(ImageLoader::FILTER_SAVE_LOCATION))
            return false;
        return this->new_filter.saveFilterInfo(ImageLoader::FILTER_SAVE_LOCATION);
        
    }

    void FilterLogic::cancelFilterCreation()
    {
        this->is_canceled = true;
        pthread_cancel(this->filter_calc_thread);
        this->filter_calc_thread = 0;
    }

    string FilterLogic::getImagePath(const size_t index)
    {
        return this->new_filter.getImagePath(index);
    }

    string FilterLogic::addImageToWorkingImages(string file_path)
    {

        return this->new_filter.loadSingleImage(file_path);
    }

    void FilterLogic::clearWorkingImages()
    {
        this->new_filter.clearWorkingImages();
    }

    void FilterLogic::cancelSaveDirtyImages()
    {
        this->new_filter.cancelSaveDirtyImages();
    }

    bool FilterLogic::saveAllDirtyImages(string folder_path)
    {
        // this->new_filter.applyNoiseToWorkingImages(this->last_noise_value, this->last_noise);
        return this->new_filter.saveAllDirtyImages(folder_path);
    }

    bool FilterLogic::saveDirtyImage(size_t image_id, string folder_path)
    {
        return this->new_filter.saveDirtyImage(image_id, folder_path);
    }

    FilterInfo *FilterLogic::getFilterInfo()
    {
        return this->new_filter.getFilterInfo();
    }

}