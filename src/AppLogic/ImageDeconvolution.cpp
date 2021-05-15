#include "ImageDeconvolution.hpp"

namespace AppLogic
{

    ImageDeconvolution::ImageDeconvolution(/* args */)
    {
        this->loadExistingFilters();
    }

    ImageDeconvolution::~ImageDeconvolution()
    {
    }

    VecImage ImageDeconvolution::applyFilterToImage(size_t image_id)
    {
        VecImage result;
         if (this->F_matrix.size() == 0 || image_id > this->working_images.size())
        {
            return result;
        }
        VecImage * w_img= &(this->working_images[image_id]);
        vec og_vec = w_img->getVecDoubleData();
        // vec filtered_vec = F_matrix * og_vec;
        result = VecImage(F_matrix * og_vec, w_img->num_cols(), w_img->num_rows());
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
            filtered_images.push_back(VecImage(filtered_vec, og_img.num_cols(), og_img.num_rows()));
        }
        return true;
    }

    vector<string> ImageDeconvolution::loadImagesFromFolder(string folder_path)
    {
        this->working_images.clear();
        ImageLoader il;

        this->working_images = il.loadImagesFromFolder(folder_path.c_str());
        
        return il.getLoadedImagePaths();
    }

    vector<string> ImageDeconvolution::loadSingleImage(string file_path)
    {
        ImageLoader il;

        this->working_images.clear();
        this->working_images.push_back(il.loadSingleImage(file_path.c_str()));

        // this->image_set = this->getWorkingImagesMat(); // can be done on a different thread to avoid blocking
        return il.getLoadedImagePaths();
    }

    void ImageDeconvolution::loadExistingFilters()
    {
        this->loaded_filters.clear();
        ImageLoader il;
        this->loaded_filters = il.loadFilterInfo(FILTER_SAVE_LOCATION);
    
    }

    bool ImageDeconvolution::loadFilter(size_t id)
    {
        if (id > this->loaded_filters.size())
        {
            return false;
        }
        this->F_matrix.reset();
        string mat_file_path = FILTER_SAVE_LOCATION + this->loaded_filters[id].filter_name + FILTER_FILE_EXTENSION;
        return this->F_matrix.load(mat_file_path);
    }

}

// +applyFilterToImage(image_id : int, filter_id : int)
// +applyFilterToWorkingImages(filter_id : int)
// +loadImagesFromFolder(folder_path : string) : string[]
// +loadImagesFromZip(file_path : string) : string[]
// +saveFilteredImage(image_id : int)
// +saveAllFilteredImages()
// +getFilteredImage(image_id : int) : char[]

// working_images : VecImage[]