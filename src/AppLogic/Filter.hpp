/**
 * @file Filter.hpp
 * @author Jorge Agüero Zamora
 * @brief Contains `Filter` class in charge of creating filters
 * @version 0.1
 * @date 2021-06-13
 * 
 */
#ifndef FILTER_H
#define FILTER_H

#define DIRTY_IMAGE_SUFFIX "_dirty.png" //!< suffix used in saved noisy image filenames

#include <vector>
#include <string>
#include <armadillo>
#include <time.h>
#include <chrono>

#include "FilterInfo.hpp"
#include "VecImage.hpp"
#include "ImageLoader.hpp"
#include "libfrcima.hpp"

namespace AppLogic
{
    using arma::mat;
    using std::string;
    using std::vector;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::seconds;

    /**
     * @brief enum with the calculation methods for the filter
     * 
     */
    typedef enum
    {
        RCIMA_METHOD,
        FAST_RCIMA_METHOD
    } calc_method_t;

    /**
     * @brief Allows to create filters based on training sets of images
     * 
     */
    class Filter
    {
    private:
        FilterInfo calc_info; //!< filter calculation information
        vector<VecImage> working_images; //!< list of loaded images 
        mat image_set; //!< training matrix of loaded images
        vector<string> loaded_file_paths; //!< paths to the loaded image files 
        vector<string> loaded_file_names; //!< names of the loaded images
        noise_type_t last_used_noise_type; //!< last noise type used
        size_t last_used_noise_value; //!< last noise value used
        bool is_canceled; //!< indicates whether filter calculation has been canceled
        mat F_matrix; //!< created filter matrix

        /**
         * @brief generates training matrix out of the loaded images
         * 
         * @return mat training matrix where each column is a vectorized form of each loaded image
         */
        mat getWorkingImagesMat();

    public:
        /**
         * @brief Construct a new Filter object
         * 
         */
        Filter();

        /**
         * @brief applies selected noise to an image and returns a new image object
         * 
         * @param image_id the index of the loaded image to apply noise to
         * @param noise_value percentage value (0-99) of amount of noise to apply
         * @param noise_type type of noise to apply to the image         
         * @return VecImage new VecImage with the applied noise
         */
        VecImage applyNoiseToImage(size_t image_id, size_t noise_value, noise_type_t noise_type = noise_type_t::GAUSSIAN);

        // to delete
        void applyNoiseToWorkingImages(size_t noise_value, noise_type_t noise_type = noise_type_t::GAUSSIAN);

        /**
         * @brief sets the name of the filter
         * 
         * @param name name of the filter
         */
        inline void setFilterName(string name)
        {
            this->calc_info.filter_name = name;
        }

        /**
         * @brief Get the Fmatrix object
         * 
         * @return mat& filter matrix
         */
        inline mat &getFmatrix()
        {
            return this->F_matrix;
        };

        /**
         * @brief Get the Filter Info object
         * 
         * @return FilterInfo* filter calculation information
         */
        inline FilterInfo *getFilterInfo()
        {
            return &this->calc_info;
        };

        /**
         * @brief removes all loaded image information
         * 
         */
        inline void clearWorkingImages()
        {
            this->working_images.clear();
            this->loaded_file_paths.clear();
            this->loaded_file_names.clear();
        }

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
         * @brief loads a single image and adds it to the working images
         * 
         * @param file_path path of the image file
         * @return string name of the loaded image
         */
        string loadSingleImage(string folder_path);

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
         * @brief returns the path of the loaded image specified by index
         * 
         * @param index the index of the loaded image to get the path for
         * @return string path of the loaded image in the filesystem
         */
        string getImagePath(size_t index);

        /**
         * @brief returns a pointer to the loaded VecImage specified by index
         * 
         * @param index the index of the loaded image to get
         * @return VecImage* pointer to the VecImage object of the loaded image
         */
        VecImage *getLoadedImage(const size_t index);

        /**
         * @brief calculates the filter
         * 
         * Calculates the filter using the last noise type and value applied, rank, and selected calculation method.
         * Sets the calculation information in `calc_info`
         * 
         * @param rank the rank to use for filter calculation. Must be a value between 1 and the minimun between the amount
         *              of images used and the size of the vectorized image.
         * @param calc_method calculation method to use. Either RCIMA or fast-RCIMA
         * @return true if calculation is successful
         * @return false if an error is encountered during calculation or calculation is canceled
         */
        bool calculateFilter(size_t rank, calc_method_t calc_method);

        /**
         * @brief saves filter matrix to file
         * 
         * @param file_path full path of the file where the matrix will be saved
         * @return true if matrix is saved successfully
         * @return false if there is an error saving the matrix
         */
        bool saveToFile(string file_path = ImageLoader::FILTER_SAVE_LOCATION);

        /**
         * @brief loads filter matrix from file
         * 
         * @param file_path full path of the file where the matrix is stored
         * @return true if matrix is loaded successfully
         * @return false if there is an error loading the matrix
         */
        bool loadFmatrixFromFile(string file_path);

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
         * @brief saves filter calculation information in the app's data directory
         * 
         * @param folder_path path where the filter_info is stored
         * @return true if filter information is saved correctly
         * @return false if there is an error saving the filter information
         */
        bool saveFilterInfo(string folder_path = ImageLoader::FILTER_SAVE_LOCATION);

    
         /**
         * @brief cancels the process of saving images with noise applied
         * 
         */
        void cancelSaveDirtyImages();

        /**
         * @brief returns a string name for the calculation method
         * 
         * @param calc_method calculation method
         * @return string name of the calculation method
         */
        static string getCalculationMethodName(calc_method_t calc_method);
    };

}

#endif
