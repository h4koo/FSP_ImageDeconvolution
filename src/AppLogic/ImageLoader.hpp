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
#include <zip.h>

#define FILTER_INFO_EXTENSION ".finfo"
#define FILTER_FILE_EXTENSION ".mat"
#define TEMP_FILE_NAME "tmpfile"
#define ZIP_FILE_BUFF_SIZE 4096


namespace AppLogic
{
    using std::string;
    using std::vector;
    using std::fstream;
    namespace fs = std::experimental::filesystem;

    class ImageLoader
    {
        private:
        vector<string> loaded_file_paths;
        vector<string> loaded_file_names;
        vector<string> not_loaded_files;
        string source_directory;
        static bool _is_canceled;
        

        inline static bool endsWith(const std::string &str, const std::string &suffix){
        return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
    }
        inline static bool isSupportedImageFormat(const string &str){
        return endsWith(str, ".jpg") || endsWith(str, ".bmp") || endsWith(str, ".png");
    }
        inline static string stripExtensionFromFilename(const string &filename){
            size_t last_index = filename.find_last_of(".");
            return filename.substr(0,last_index);
        }
         void clearLoadedData(){
            loaded_file_paths.clear();
            loaded_file_names.clear();
            not_loaded_files.clear();

        }
        void addImageFileInfo(const string &full_path);
        static bool createFileFromZipFile(string filename, fstream &stream, zip_file *zip_file, size_t buff_size);


    public:
        // ImageLoader();
        // ~ImageLoader();
        const static string FILTER_SAVE_LOCATION;
        VecImage loadSingleImage(const char *filename);
        vector<VecImage> loadImagesFromFolder(const char *folder_path);
        vector<string> getLoadedImageNames();
        vector<string> getLoadedImagePaths();
        vector<string> getNotLoadedImagePaths();
        string getSourceDirectory();
        vector<FilterInfo> loadFilterInfo();
        static bool deleteFilter(const string filter_name);
        static bool existsNameFilter(string filter_name);
        vector<VecImage> loadImagesFromZip(string file_path);
        static bool createExportZip(string export_filename, string filter_name);
        static bool importFilterFromZip(string import_filename);
        static void cancelOperation(bool status);
    
    
    };

}

#endif