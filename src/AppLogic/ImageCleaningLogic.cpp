#include "ImageCleaningLogic.hpp"

namespace AppLogic
{
    vector<string> ImageCleaningLogic::getFilterNames()
    {

        this->filters = this->deconv.loadExistingFilters();
        vector<string> result;
        for (auto &&filt : this->filters)
        {
            result.push_back(filt.filter_name);
        }

        return result;
    }

    vector<string> ImageCleaningLogic::loadImagesFromFolder(string folder_path)
    {

        return this->deconv.loadImagesFromFolder(folder_path);
    }

    vector<string> ImageCleaningLogic::loadImagesFromZip(string file_path)
    {
        return this->deconv.loadImagesFromZip(file_path);
    }

    VecImage *ImageCleaningLogic::getLoadedImage(const size_t index)
    {
        return this->deconv.getLoadedImage(index);
    }

    bool ImageCleaningLogic::loadFilter(const string filtername)
    {
        return this->deconv.loadFilter(this->deconv.getFilterIndex(filtername));
    }

    VecImage ImageCleaningLogic::applyFilterToImage(int image_id)
    {
        return this->deconv.applyFilterToImage(image_id);
    }

    FilterInfo *ImageCleaningLogic::getFilterInfo(const string filtername)
    {
       return this->deconv.getFilterInfo(this->deconv.getFilterIndex(filtername));
    }

    string ImageCleaningLogic::getImagePath(const size_t index)
    {
        return this->deconv.getImagePath(index);
    }
    string ImageCleaningLogic::addImageToWorkingImages(string file_path)
    {

        return this->deconv.loadSingleImage(file_path);
    }

    void ImageCleaningLogic::clearWorkingImages()
    {
        this->deconv.clearWorkingImages();
    }

    bool ImageCleaningLogic::saveAllFilteredImages(string folder_path)
    {
        return this->deconv.saveAllFilteredImages(folder_path);
    }

    bool ImageCleaningLogic::saveFilteredImage(size_t image_id, string folder_path)
    {
        return this->deconv.saveFilteredImage(image_id, folder_path);
    }

    bool ImageCleaningLogic::deleteFilter(const string filtername)
    {
        return this->deconv.deleteFilter(this->deconv.getFilterIndex(filtername));
    }

    // bool ImageCleaningLogic::exportFilter(size_t index, const string path_to_file){
    //     return this->deconv.exportFilter(index, path_to_file);
    // }
    // bool ImageCleaningLogic::importFilter(const string path_to_file){
    //     return this->deconv.importFilter(path_to_file);
    // }

    void ImageCleaningLogic::cancelFilterAllImages()
    {
        this->deconv.cancelFilterAllImages();
    }
}