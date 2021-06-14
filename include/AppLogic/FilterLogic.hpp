/**
 * @file FilterLogic.hpp
 * @author Jorge Agüero Zamora
 * @brief Contains the API for filter creation
 * @version 0.1
 * @date 2021-06-13
 * 
 * 
 */

#ifndef FILTERLOGIC_H
#define FILTERLOGIC_H

#include "Filter.hpp"
#include <pthread.h>
#include <thread>

namespace AppLogic
{
    using std::string;
    using std::vector;

    /**
     * @brief API to the Filter class
     * 
     * Used to provide the functionality of loading images and creating filters
     * out of the loaded images. Allows users to apply noise to images and save the dirty images.
     * 
     */
    class FilterLogic
    {
    private:
        Filter new_filter; //!< Filter object to use
        noise_type_t last_noise; //!< last noise type used
        double last_noise_value; //!< last noise value used
        pthread_t filter_calc_thread; //!< thread for calculating the filter
        bool is_canceled = false; //!< indicates whether filter calculation has been canceled

    public:
        /**
         * @brief Construct a new Filter Logic object
         * 
         */
        FilterLogic();

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
         * @brief returns a pointer to the loaded VecImage specified by index
         * 
         * @param index the index of the loaded image to get
         * @return VecImage* pointer to the VecImage object of the loaded image
         */
        VecImage *getLoadedImage(const size_t index);

        /**
         * @brief applies selected noise to an image and returns a new image object
         * 
         * @param image_id the index of the loaded image to apply noise to
         * @param noise type of noise to apply to the image
         * @param noise_value percentage value (0-99) of amount of noise to apply
         * @return VecImage new VecImage with the applied noise
         */
        VecImage applyNoiseToImage(int image_id, noise_type_t noise, double noise_value);

        /**
         * @brief calculates the filter
         * 
         * Calculates the filter using the last noise type and value applied and the 
         * selected name, rank, and calculation method
         * 
         * @param name name of the filter. A file is created with the filter name, no validation is done on the name 
         * @param rank the rank to use for filter calculation. Must be a value between 1 and the minimun between the amount
         *              of images used and the size of the vectorized image.
         * @param calc_method calculation method to use. Either RCIMA or fast-RCIMA
         * @return true if calculation is successful
         * @return false if an error is encountered during calculation or calculation is canceled
         */
        bool createFilter(string name, int rank, calc_method_t calc_method);

        /**
         * @brief cancels the filter calculation
         * 
         */
        void cancelFilterCreation();

        /**
         * @brief sets the value of the last used noise type
         * 
         * @param n_type type of noise
         */
        void setNoise(noise_type_t n_type)
        {
            this->last_noise = n_type;
        };

        /**
         * @brief sets the last noise value used
         * 
         * @param val value in range [0,99] indicating the percentage of noise
         */
        void setNoiseValue(double val)
        {
            this->last_noise_value = val;
        };

        /**
         * @brief applies noise to all working images and saves them in the specified path
         * 
         * Applies noise to all loaded images using the last noise type and value applied and
         * saves them to the folder path provided as PNG (.png) images. This operation does not
         * modify the loaded images.
         * 
         * @param folder_path path where the images will be saved
         * @return true if images are saved correctly
         * @return false if an eror is encountered or saving process is canceled
         */
        bool saveAllDirtyImages(string folder_path);

        /**
         * @brief cancels the process of saving images with noise applied
         * 
         */
        void cancelSaveDirtyImages();

        /**
         * @brief applies noise to image and saves it in the specified path
         * 
         * Applies noise to the loaded image specified by the index image_id, using the 
         * last noise type and value applied and saves it to the folder path provided 
         * as a PNG (.png) image. This operation does not modify the loaded image.
         * 
         * @param image_id index of the image to apply noise to and save
         * @param folder_path name of the file to save the dirty image
         * @return true if image is saved corectly
         * @return false if there is an error saving the image
         */
        bool saveDirtyImage(size_t image_id, string folder_path);

        /**
         * @brief returns pointer to the `FilterInfo` object fo current filter.
         * 
         * @return FilterInfo* Object containing the calculation information for the curent filter
         */
        FilterInfo *getFilterInfo();
    };
}

#endif
