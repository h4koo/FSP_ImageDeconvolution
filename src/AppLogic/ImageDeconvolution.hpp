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
        mat F_matrix;

        void loadExistingFilters();

    public:
        ImageDeconvolution(/* args */);
        ~ImageDeconvolution();
    

   vector<FilterInfo> getLoadedFilters(){
       return this->loaded_filters;
   };

    VecImage  applyFilterToImage(size_t image_id);

    bool applyFilterToWorkingImages();
    bool loadFilter(size_t id);

    vector<string> loadImagesFromFolder(string folder_path);
    vector<string> loadSingleImage(string file_path);
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