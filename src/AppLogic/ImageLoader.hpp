/**
 * @file ImageLoader.hpp
 * @author Jorge Agüero Zamora
 * @brief Contains ImageLoader class in charge of loading images and filter information
 * @version 0.1
 * @date 2021-06-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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


#define FILTER_INFO_EXTENSION ".finfo"          //!< file extension for FilterInfo files
#define FILTER_FILE_EXTENSION ".mat"            //!<file extension for filter matrix files
#define TEMP_FILE_NAME "tmpfile"                //!< name of tempfile to use when extracting from zip
#define ZIP_FILE_BUFF_SIZE 4096                 //!< size of the buffer to copy data from zip


namespace AppLogic
{
    using std::string;
    using std::vector;
    using std::fstream;
    namespace fs = std::experimental::filesystem;

    /**
     * @brief Allows to load images and filter information as well as import and export filters
     * 
     */
    class ImageLoader
    {
        private:
        vector<string> loaded_file_paths; //!< path of the files for the loaded images
        vector<string> loaded_file_names; //!< names of the loaded images
        vector<string> not_loaded_files; //!< list of not loaded image names
        string source_directory; //!< name of the folder from where the images where loaded
        static bool _is_canceled; //!< indicates whether image loading/import/export operation has been canceled
        

        /**
         * @brief checks whether a string ends with supplied suffinx
         * 
         * @param str string to check
         * @param suffix suffix to check
         * @return true if string ends in suffix
         * @return false is string ends in something different than suffix
         */
        inline static bool endsWith(const std::string &str, const std::string &suffix){
        return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
    }
        /**
         * @brief checks whether a provided image filename is a supported format
         * 
         * @param str name of the image file
         * @return true if the name of the file ends in one of the supported image format extensions
         * @return false if the name of the file does not ends in a supported extension
         */
        inline static bool isSupportedImageFormat(const string &str){
        return endsWith(str, ".jpg") || endsWith(str, ".bmp") || endsWith(str, ".png");
    }
        /**
         * @brief removes file extension from filename
         * 
         * This function removes everything after the last dot (.) encountered 
         * in the filename
         * 
         * @param filename name of the file with extension
         * @return string name of the file without the extension
         */
        inline static string stripExtensionFromFilename(const string &filename){
            size_t last_index = filename.find_last_of(".");
            return filename.substr(0,last_index);
        }

        /**
         * @brief clears all loaded information
         * 
         */
         void clearLoadedData(){
            loaded_file_paths.clear();
            loaded_file_names.clear();
            not_loaded_files.clear();

        }

        /**
         * @brief adds image information to the loaded images lists
         * 
         * @param full_path path of the image file
         */
        void addImageFileInfo(const string &full_path);

        /**
         * @brief Create a file from Zip file object
         * 
         * @param filename 
         * @param stream 
         * @param zip_file 
         * @return true 
         * @return false 
         */
        static bool createFileFromZipFile(string filename, fstream &stream, zip_file *zip_file);


    public:


        const static string FILTER_SAVE_LOCATION; //!< Application's data folder in Linux filesystem

        /**
         * @brief loads image from file and returns a VecImage
         * 
         * @param filename path of the image file
         * @return VecImage loaded image
         */
        VecImage loadSingleImage(const char *filename);

        /**
         * @brief loads images from specified full path to a folder
         * 
         * This function will look for the first valid image in the folder and will load 
         * all other valid images of the same resolution as that first loaded image, other 
         * images are ignored.
         * 
         * @param folder_path full path to the directory containing the images
         * @return vector<VecImage> list of loaded images
         */
        vector<VecImage> loadImagesFromFolder(const char *folder_path);

        /**
         * @brief returns the list of the loaded image names
         * 
         * @return vector<string> list of the loaded image names
         */
        vector<string> getLoadedImageNames();

        /**
         * @brief returns the list of the loaded image paths
         * 
         * @return vector<string> list of the loaded image paths
         */
        vector<string> getLoadedImagePaths();

        /**
         * @brief returns the list of image names that were not loaded
         * 
         * @return vector<string> 
         */
        vector<string> getNotLoadedImagePaths();

        /**
         * @brief returns the source directory from where the images where loaded
         * 
         * @return string source directory from where the images where loaded
         */
        string getSourceDirectory();

        /**
         * @brief checks the app's data folder for existing filter info files
         * 
         * @return vector<FilterInfo> list of FilterInfo objects found in filesystem
         */
        vector<FilterInfo> loadFilterInfo();

        /**
         * @brief deletes filter from the app's data folder
         * 
         * This deletion is linux specific since it uses OS directives to remove the files
         * 
         * @param filter_name name of the filter to delete
         * @return true if the deletion is successful
         * @return false if there is a problem deleting the filter
         */
        static bool deleteFilter(const string filter_name);

        /**
         * @brief checks if a filter with the provided name already exists in the app's data folder
         * 
         * @param filter_name name of the filter to check
         * @return true if a filter with the same name already exists
         * @return false if there is no filter with the same name
         */
        static bool existsNameFilter(string filter_name);

        /**
         * @brief load images from a zip file
         * 
         * This function will go through all files in a zip file. It  will look for the first 
         * valid image in the folder and will load all other valid images of the same resolution
         * as the first loaded image, other images are ignored.
         * 
         * @param file_path full path of a valid zip file 
         * @return vector<VecImage> list of loaded images
         */
        vector<VecImage> loadImagesFromZip(string file_path);

        /**
         * @brief creates an export Zip file
         * 
         * Creates a zip file with both the *.mat and *.finfo files that contain the filter information
         * 
         * @param export_filename name of the zip file to create
         * @param filter_name name of the filter to be exported
         * @return true if filter is exported successfully
         * @return false if there is a problem exporting the filter
         */
        static bool createExportZip(string export_filename, string filter_name);

        /**
         * @brief imports an exported filter from a zip file
         * 
         * @param import_filename name of the zip file with the exported filter
         * @return true if filter is imported successfully
         * @return false if there is a problem importing the filter 
         */
        static bool importFilterFromZip(string import_filename);

        /**
         * @brief sets cancel status 
         * 
         * Cancels the loading operations and the export/import operations.
         * 
         * @param status cancel status
         */
        static void cancelOperation(bool status);
    
    
    };

}

#endif