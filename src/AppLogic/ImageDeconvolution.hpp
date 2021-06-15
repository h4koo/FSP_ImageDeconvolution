/**
 * @file ImageDeconvolution.hpp
 * @author Jorge Agüero Zamora
 * @brief Contains `ImageDeconvolution` class in charge of applying filters to images
 * @version 0.1
 * @date 2021-06-13
 * 
 */
#ifndef IMAGEDECONVOLUTION_H
#define IMAGEDECONVOLUTION_H

#define FILT_IMAGE_SUFFIX "_filtered.png" //!< suffix used in saved filtered image filenames

#include <vector>
#include <string>
#include <armadillo>

#include "FilterInfo.hpp"
#include "VecImage.hpp"
#include "ImageLoader.hpp"

namespace AppLogic
{
    /**
     * @brief Allows to apply existing filter to images
     * 
     */
    class ImageDeconvolution
    {
    private:
        vector<FilterInfo> loaded_filters; //!< filter information of existing filters
        vector<VecImage> working_images; //!< list of loaded images
        // vector<VecImage> filtered_images; //!< lisf of filtered images
        vector<string> loaded_file_paths; //!< paths to the loaded image files 
        vector<string> loaded_file_names; //!< names of the loaded images
        bool is_canceled; //!< indicates whether image saving process is canceled
        mat F_matrix; //!< Filter matrix

    public:

        /**
         * @brief Construct a new Image Deconvolution object
         * 
         */
        ImageDeconvolution();

        /**
         * @brief Destroy the Image Deconvolution object
         * 
         */
        ~ImageDeconvolution();

        /**
         * @brief deletes all loaded image information
         * 
         */
        inline void clearWorkingImages()
        {
            this->working_images.clear();
            this->loaded_file_paths.clear();
            this->loaded_file_names.clear();
        }

        /**
         * @brief returns the path of the loaded image specified by index
         * 
         * @param index the index of the loaded image to get the path for
         * @return string path of the loaded image in the filesystem
         */
        string getImagePath(size_t index);

        /**
         * @brief Get the Loaded Filters object
         * 
         * @return vector<FilterInfo> list of FilterInfo objects containing the information of the existing filters
         */
        inline vector<FilterInfo> getLoadedFilters()
        {
            return this->loaded_filters;
        };

        /**
         * @brief returns a new image of the selected image with the filter applied.
         * 
         * Aplies the current folder to the image identified by `image_id` 
         * 
         * @param image_id index of the image to apply the filter to
         * @return VecImage new `VecImage` with the filter applied
         */
        VecImage applyFilterToImage(size_t image_id);

        /**
         * @brief returns a pointer to the loaded `VecImage` specified by `index`
         * 
         * @param index the index of the loaded image to get
         * @return VecImage* pointer to the `VecImage` object of the loaded image
         */
        VecImage * getLoadedImage(const size_t index);


        // bool applyFilterToWorkingImages();

        /**
         * @brief load filter matrix from file
         * 
         * Looks for the corresponding *.mat file with the same
         * name as the filter identified by the index `id`,
         * and loads the matrix to be used to apply to images.
         * 
         * @param id index of the `FilterInfo` in `loaded_filters` with the name of the filter to be loaded
         * @return true if the matrix is loaded correctly
         * @return false if there is an error loading the matrix
         */
        bool loadFilter(int index);

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
         * @brief loads a single image and adds it to the working images
         * 
         * @param file_path path of the image file
         * @return string name of the loaded image
         */
        string loadSingleImage(string file_path);

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
        bool deleteFilter(int index);
       
        /**
         * @brief cancels the process of saving images with filter applied
         * 
         */
        void cancelFilterAllImages();

        /**
         * @brief checks application data directory in order to load existing filter information
         * 
         * @return vector<FilterInfo> list of FilterInfo objects for existing filters in filesystem
         */
        vector<FilterInfo> loadExistingFilters();

        /**
         * @brief returns pointer to the FilterInfo object fo current filter.
         * 
         * @return FilterInfo* Object containing the calculation information for the curent filter
         */
        FilterInfo *getFilterInfo(int index);

        int getFilterIndex(const string filtername);

         // bool exportFilter(size_t index, const string path_to_file);
        // bool importFilter(const string path_to_file);


    };
}

#endif