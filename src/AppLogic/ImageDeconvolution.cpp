#include "ImageDeconvolution.hpp"
#define FILT_IMAGE_SUFFIX "_filtered.png"

namespace AppLogic
{

    ImageDeconvolution::ImageDeconvolution() : is_canceled(false)
    {
    }

    ImageDeconvolution::~ImageDeconvolution()
    {
    }
    string ImageDeconvolution::getImagePath(size_t index)
    {
        if (index > this->loaded_file_paths.size())
            index = 0;
        return this->loaded_file_paths[index];
    }

    VecImage ImageDeconvolution::applyFilterToImage(size_t image_id)
    {
        VecImage result;

        if (this->F_matrix.size() == 0 || image_id > this->working_images.size())
        {
            return result;
        }
        VecImage *w_img = &(this->working_images[image_id]);
        vec og_vec = w_img->getVecDoubleData();
        if (this->F_matrix.n_cols != og_vec.size())
            return result;

        result = VecImage(F_matrix * og_vec, w_img->numCols(), w_img->numRows());
        return result;
    }

    bool ImageDeconvolution::applyFilterToWorkingImages()
    {

        if (this->F_matrix.size() == 0 || this->working_images.size() == 0)
        {
            return false;
        }

        this->filtered_images.clear();
        for (auto &&og_img : working_images)
        {
            vec og_vec = og_img.getVecDoubleData();
            vec filtered_vec = F_matrix * og_vec;
            filtered_images.push_back(VecImage(filtered_vec, og_img.numCols(), og_img.numRows()));
        }
        return true;
    }

    vector<string> ImageDeconvolution::loadImagesFromZip(string file_path)
    {
        this->working_images.clear();
        ImageLoader il;

        this->working_images = il.loadImagesFromZip(file_path.c_str());
        this->loaded_file_names = il.getLoadedImageNames();
        this->loaded_file_paths = il.getLoadedImagePaths();

        return this->loaded_file_names;
    }

    vector<string> ImageDeconvolution::loadImagesFromFolder(string folder_path)
    {
        this->working_images.clear();
        ImageLoader il;

        this->working_images = il.loadImagesFromFolder(folder_path.c_str());
        this->loaded_file_names = il.getLoadedImageNames();
        this->loaded_file_paths = il.getLoadedImagePaths();

        return this->loaded_file_names;
    }

    string ImageDeconvolution::loadSingleImage(string file_path)
    {
        ImageLoader il;

        this->working_images.push_back(il.loadSingleImage(file_path.c_str()));
        this->loaded_file_names.push_back(il.getLoadedImageNames()[0]);
        this->loaded_file_paths.push_back(il.getLoadedImagePaths()[0]);

        return this->loaded_file_names.back();
    }

    vector<FilterInfo> ImageDeconvolution::loadExistingFilters()
    {
        this->loaded_filters.clear();
        ImageLoader il;
        this->loaded_filters = il.loadFilterInfo();
        return this->loaded_filters;
    }

    bool ImageDeconvolution::loadFilter(size_t id)
    {
        if (id > this->loaded_filters.size())
            return false;

        this->F_matrix.reset();
        string mat_file_path = ImageLoader::FILTER_SAVE_LOCATION + this->loaded_filters[id].filter_name + FILTER_FILE_EXTENSION;
        return this->F_matrix.load(mat_file_path);
    }

    bool ImageDeconvolution::saveFilteredImage(size_t image_id, string folder_path)
    {
        if (image_id > this->working_images.size())
            return false;
        return this->applyFilterToImage(image_id).save(folder_path);
    }

    bool ImageDeconvolution::saveAllFilteredImages(string folder_path)
    {
        for (size_t index = 0; index < this->working_images.size(); ++index)
        {
            if (this->is_canceled)
            {
                this->is_canceled = false;
                return false;
            }

            string filename = folder_path + "/" + this->loaded_file_names[index] + FILT_IMAGE_SUFFIX;
            if (!this->applyFilterToImage(index).save(filename))
                return false;
        }
        return true;
    }

    void ImageDeconvolution::cancelFilterAllImages()
    {
        this->is_canceled = true;
    }

    bool ImageDeconvolution::deleteFilter(size_t index)
    {
        if (index > this->loaded_filters.size())
            return false;
        FilterInfo *fi = &this->loaded_filters[index];
        if (!ImageLoader::deleteFilter(fi->filter_name))
            return false;

        // delete loaded filter
        this->loaded_filters.erase(this->loaded_filters.begin() + index);

        return true;
    }

    // bool ImageDeconvolution::exportFilter(size_t index, const string path_to_file){

    // }
    // bool ImageDeconvolution::importFilter(const string path_to_file){

    // }

    VecImage *ImageDeconvolution::getLoadedImage(const size_t index)
    {
        if (index > this->working_images.size())
            return NULL;
        return &(this->working_images[index]);
    }

}
