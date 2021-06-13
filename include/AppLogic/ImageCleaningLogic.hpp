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
        vector<string> loadImagesFromFolder(string folder_path);
        vector<string> loadImagesFromZip(string file_path);
        string addImageToWorkingImages(string file_path);
        void clearWorkingImages();

        string getImagePath(const size_t index);
        bool loadFilter(int filter_id);

        VecImage *getLoadedImage(const size_t index);
        VecImage applyFilterToImage(int image_id);

        FilterInfo *getFilterInfo(size_t indx);

        bool saveAllFilteredImages(string folder_path);
        bool saveFilteredImage(size_t image_id, string folder_path);

        bool deleteFilter(size_t index);

        // bool exportFilter(size_t index, const string path_to_file);
        // bool importFilter(const string path_to_file);
        void cancelFilterAllImages();
    };

}

#endif
