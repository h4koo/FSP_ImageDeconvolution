#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "VecImage.hpp"
#include "FilterInfo.hpp"
#include "CImg.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>

#define FILTER_SAVE_LOCATION "/mnt/c/Users/JorgeAgueroZamora/Documents/TEC/ProyectoDiseno/FroSigProImageDeconvolution/FSP_ImageDeconvolution/test/"
#define FILTER_INFO_EXTENSION ".finfo"
#define FILTER_FILE_EXTENSION ".mat"

namespace AppLogic
{
    using std::string;
    using std::vector;
    using std::fstream;

    class ImageLoader
    {
        private:
        vector<string> loaded_file_paths;
        vector<string> loaded_file_names;
        vector<string> not_loaded_files;
        string source_directory;

        static bool endsWith(const std::string &str, const std::string &suffix){
        return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
    }
        static bool isSupportedImageFormat(const string &str){
        return endsWith(str, ".jpg") || endsWith(str, ".bmp") || endsWith(str, ".png");
    }
         void clearLoadedData(){
            loaded_file_paths.clear();
            loaded_file_names.clear();
            not_loaded_files.clear();

        }


    public:
        VecImage loadSingleImage(const char *filename);
        vector<VecImage> loadImagesFromFolder(const char *folder_path);
        vector<string> getLoadedImageNames();
        vector<string> getLoadedImagePaths();
        vector<string> getNotLoadedImagePaths();
        string getSourceDirectory();
        vector<FilterInfo> loadFilterInfo(const char *folder_path);



        // +loadImagesFromZip(file_path) : VecImage[]
        // +loadSingleImage(file_path : string) : VecImage
        // +saveImageToFile(file_path : string, image : VecImage) : bool
        // +saveMultipleImagesToFile(file_path : string, image : VecImage) : bool
    };

}

#endif