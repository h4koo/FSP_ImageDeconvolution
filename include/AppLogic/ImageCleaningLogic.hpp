#ifndef IMAGECLEANINGLOGIC_H
#define IMAGECLEANINGLOGIC_H


#include "ImageDeconvolution.hpp"

namespace AppLogic
{

    using std::string;
    using std::vector;

    class ImageCleaningLogic
    {
    private:
        ImageDeconvolution deconv;
        vector<FilterInfo> filters;

    public:
        vector<string> getFilterNames();
        vector<string> loadSingleImage(string file_path);
        vector<string> loadImagesFromFolder(string folder_path);
        bool loadFilter(int filter_id);

        VecImage applyFilterToImage(int image_id);
        FilterInfo * getFilterInfo(size_t indx);
    };

}

#endif

// +loadImagesFromFolder(folder_path : string) : string[]
// +loadImagesFromZip(file_path : string) : string[]
// +loadSingleImage(file_path : string) : string[]
// +getFilterList() : FilterInfo[]
// +applyFilterToImage(image_id : int, filter_id : int)
// +applyFilterToAllImages(filter_id : int)
// +getFilteredImage(image_id : int) : char[]
// +saveFilteredImage()
// +saveAllFilteredImages()

// -deconvolutioner : ImageDeconvolution
// -filters : FilterInfo[]
