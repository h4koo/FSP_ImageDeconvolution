#ifndef IMAGEDECONVOLUTION_H
#define IMAGEDECONVOLUTION_H

#include <vector>
#include <string>
#include <armadillo>

#include "FilterInfo.hpp"
#include "VecImage.hpp"
#include "ImageLoader.hpp"

namespace AppLogic
{

    class ImageDeconvolution
    {
    private:
        vector<FilterInfo> loaded_filters;
        vector<VecImage> working_images;
        vector<VecImage> filtered_images;
        vector<string> loaded_file_paths;
        vector<string> loaded_file_names;
        bool is_canceled;
        mat F_matrix;

        

    public:
        ImageDeconvolution(/* args */);
        ~ImageDeconvolution();

        inline void clearWorkingImages()
        {
            this->working_images.clear();
            this->loaded_file_paths.clear();
            this->loaded_file_names.clear();
        }

        string getImagePath(size_t index);
        inline vector<FilterInfo> getLoadedFilters()
        {
            return this->loaded_filters;
        };

        VecImage applyFilterToImage(size_t image_id);
        VecImage * getLoadedImage(const size_t index);

        bool applyFilterToWorkingImages();
        bool loadFilter(size_t id);
        bool saveFilteredImage(size_t image_id, string folder_path);
        bool saveAllFilteredImages(string folder_path);
        vector<string> loadImagesFromFolder(string folder_path);
        vector<string> loadImagesFromZip(string file_path);
        string loadSingleImage(string file_path);

        bool deleteFilter(size_t index);
        // bool exportFilter(size_t index, const string path_to_file);
        // bool importFilter(const string path_to_file);
        void cancelFilterAllImages();
        vector<FilterInfo> loadExistingFilters();


    };
}

#endif
// +applyFilterToImage(image_id : int, filter_id : int)
// +applyFilterToWorkingImages(filter_id : int)
// +loadImagesFromFolder(folder_path : string) : string[]
// +loadImagesFromZip(file_path : string) : string[]
// +saveFilteredImage(image_id : int)
// +saveAllFilteredImages()
// +getFilteredImage(image_id : int) : char[]

// working_images : VecImage[]