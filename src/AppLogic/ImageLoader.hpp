#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "VecImage.hpp"

#include "CImg.h"
#include <string>
#include <vector>

namespace AppLogic
{
    using std::string;
    using std::vector;

    class ImageLoader
    {
        vector<string> loaded_images;
        vector<string> unloaded_images;

    public:
        VecImage loadSingleImage(const char *filename);
        vector<VecImage> loadImagesFromFolder(const char *folder_path);
        vector<string> getLoadedImageNames();

        // +loadImagesFromZip(file_path) : VecImage[]
        // +loadSingleImage(file_path : string) : VecImage
        // +saveImageToFile(file_path : string, image : VecImage) : bool
        // +saveMultipleImagesToFile(file_path : string, image : VecImage) : bool
    };

}

#endif