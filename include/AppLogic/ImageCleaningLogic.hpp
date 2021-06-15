/**
 * @file ImageCleaningLogic.hpp
 * @author Jorge Agüero Zamora
 * @brief Contains the API to be able to apply filters to images
 * @version 0.1
 * @date 2021-06-13
 * 
 */
#ifndef IMAGECLEANINGLOGIC_H
#define IMAGECLEANINGLOGIC_H

#include "ImageDeconvolution.hpp"

namespace AppLogic
{

    using std::string;
    using std::vector;

    /**
     * @brief API to the ImageDeconvolution class
     *
     * Used to load existing filters and apply filters to loaded images.
     * Allows users to load images, apply already created filters to the 
     * images and save those images
     * 
     */
    class ImageCleaningLogic
    {
    private:
        ImageDeconvolution deconv; //!< ImageDeconvolution object that provides functionality
        vector<FilterInfo> filters; //!< list of FilterInfo objects containing the information of the filters found on file

    public:
        /**
         * @brief returns a list with the names of the filters found on file.
         * 
         * Checks the application data folder looking for *.finfo files and gets the name of the filters.
         * 
         * @return vector<string> list of names of filters found in application data folder
         */
        vector<string> getFilterNames();

        /**
         * @brief loads images from specified full path to a folder
         * 
         * This function will look for the first valid image in the folder and will load 
         * all other valid images of the same resolution as that first loaded image, other 
         * images are ignored.
         * 
         * @param folder_path full path to the directory containing the images
         * @return vector<string> list of names of the images that were loaded
         */
        vector<string> loadImagesFromFolder(string folder_path);

        /**
         * @brief load images from a zip file
         * 
         * This function will go through all files in a zip file. It  will look for the first 
         * valid image in the folder and will load all other valid images of the same resolution
         * as the first loaded image, other images are ignored.
         * 
         * @param file_path full path of a valid zip file
         * @return vector<string> list of names of the images that were loaded
         */
        vector<string> loadImagesFromZip(string file_path);

        /**
         * @brief loads one image and adds it the the list of working images
         * 
         * @param file_path full path of a valid image
         * @return string returns the name of the loaded image
         */
        string addImageToWorkingImages(string file_path);

        /**
         * @brief deletes all loaded images 
         * 
         */
        void clearWorkingImages();

        /**
         * @brief returns the path of the loaded image specified by index
         * 
         * @param index the index of the loaded image to get the path for
         * @return string path of the loaded image in the filesystem
         */
        string getImagePath(const size_t index);

        /**
         * @brief load filter matrix from file
         * 
         * Looks for the corresponding *.mat file with the same
         * name as the filter identified by the index `filter_id`,
         * and loads the matrix to be used to apply to images.
         * 
         * @param filter_id index of the `FilterInfo` in `filters` with the name of the filter to be loaded
         * @return true if the matrix is loaded correctly
         * @return false if there is an error loading the matrix
         */
        bool loadFilter(const string filtername);

        /**
         * @brief returns a pointer to the loaded `VecImage` specified by `index`
         * 
         * @param index the index of the loaded image to get
         * @return VecImage* pointer to the `VecImage` object of the loaded image
         */
        VecImage *getLoadedImage(const size_t index);

        /**
         * @brief returns a new image of the selected image with the filter applied.
         * 
         * Aplies the current folder to the image identified by `image_id` 
         * 
         * @param image_id index of the image to apply the filter to
         * @return VecImage new VecImage with the filter applied
         */
        VecImage applyFilterToImage(int image_id);

         /**
         * @brief returns pointer to the FilterInfo object fo current filter.
         * 
         * @return FilterInfo* Object containing the calculation information for the curent filter
         */
        FilterInfo *getFilterInfo(const string filtername);

        /**
         * @brief applies filter to all working images and saves them in the specified path
         * 
         * Applies filter to all loaded images using the loded filter and
         * saves them to the folder path provided as PNG (.png) images. This operation does not
         * modify the loaded images.
         * 
         * @param folder_path path where the images will be saved
         * @return true if images are saved correctly
         * @return false if an eror is encountered or saving process is canceled
         */
        bool saveAllFilteredImages(string folder_path);

        /**
         * @brief applies filter to image and saves it in the specified path
         * 
         * Applies filter to the loaded image specified by the index `image_id`, using the 
         * loaded filter and saves it to the folder path provided 
         * as a PNG (.png) image. This operation does not modify the loaded image.
         * 
         * @param image_id index of the image to apply noise to and save
         * @param folder_path name of the file to save the dirty image
         * @return true if image is saved corectly
         * @return false if there is an error saving the image
         */
        bool saveFilteredImage(size_t image_id, string folder_path);

        /**
         * @brief deletes filter specified by `index`
         * 
         * Checks the application data folder in order to delete the *.finfo and *.mat files
         * that ath the name of the filter specified by `index` in `filters`.
         * 
         * @param index index of the `FilterInfo` that has the name of the filter to delete
         * @return true if the filter is deleted successfully
         * @return false if there is an error deleting the filters
         */
        bool deleteFilter(const string filtername);

        /**
         * @brief cancels the process of saving images with filter applied
         * 
         */
        void cancelFilterAllImages();

        // bool exportFilter(size_t index, const string path_to_file);
        // bool importFilter(const string path_to_file);



        
    };

}

#endif
