/**
 * @file VecImage.hpp
 * @author Jorge Agüero Zamora
 * @brief Contains the `VecImage` class used to manipulate images
 * @version 0.1
 * @date 2021-06-13
 * 
 */
#ifndef VECIMAGE_H
#define VECIMAGE_H

#define cimg_use_jpeg
#define cimg_use_png
#define cimg_display 0 // !! Important: Disables X11 libs so project can build with QT !!

#include <string>
#include <vector>
#include <armadillo>
#include <math.h>

#include "CImg.h"

namespace AppLogic
{
    using arma::mat;
    using arma::vec;
    using cimg_library::CImg;
    using std::string;
    using std::vector;

    /**
     * @brief enum with the different type of noises that can be applied to a `VecImage`
     * 
     */
    typedef enum
    {
        GAUSSIAN,
        SALT_PEPPER,
        UNIFORM,
        POISSON,
        RICIAN
    } noise_type_t;

    /**
     * @brief Class used for image manipulation
     * 
     * Allows users to create images, load images from folder, and get the underlying double
     * values of the image in order to calculate filters and apply filters to the images.
     * 
     */
    class VecImage
    {
    private:
        string source_file_location; //!< path of the image in the filesystem when it's loaded from file
        size_t cols; //!< number of columns in the image
        size_t rows; //!< number of rows in the image
        cimg_library::CImg<unsigned char> data; //!< the image data

    public:
        /**
         * @brief empty image constructor
         * 
         */
        VecImage();

        /**
         * @brief constructs a new `VecImage` by loading it from file
         * 
         * Loads an image from file and creates a new image in grayscale.
         * Images must be in JPG or PNG format.
         * 
         * @param filename path of the image to load
         */
        VecImage(string filename);

        /**
         * @brief construct a new image from an std::vector containing the image data
         * 
         * Constructs a new grayscale image from the `img_data` vector and  `width` and 
         * `height` specified.
         * 
         * @param img_data vector containig normalized values [0,1] of the pixel data of the image
         * @param width  horizontal size of the image
         * @param height vertical size of the image
         */
        VecImage(vector<double> &img_data, size_t width, size_t height);

        /**
         * @brief construct a new image from an armadillo::vector containing the image data
         * 
         * Constructs a new grayscale image from the `img_data` vector and  `width` and 
         * `height` specified.
         * 
         * @param img_data vector containig normalized values [0,1] of the pixel data of the image
         * @param width  horizontal size of the image
         * @param height vertical size of the image
         */
        VecImage(const vec &img_data, size_t width, size_t height);

        /**
         * @brief constructs `VecImage` from a `CImg` object
         * 
         * @param cImg CImg to be used as source for new the image
         */
        VecImage(CImg<unsigned char> &cImg);

        /**
         * @brief copy constructor for `VecImage`
         * 
         * @param vImg image to copy data from
         */
        VecImage(const VecImage &vImg);

        /**
         * @brief assignment operator, copies values from one image to the other
         * 
         * @param image 
         * @return VecImage& 
         */
        VecImage& operator= (const VecImage &image);

        /**
         * @brief returns the number of rows (height) of the image
         * 
         * @return size_t number of rows (height) of the image
         */
        size_t numRows();

        /**
         * @brief returns the number of columns (width) of the image
         * 
         * @return size_t number of columns (width) of the image
         */
        size_t numCols(); 

        /**
         * @brief returns the path from where the image was loaded
         * 
         * If the image was created in the application and not loaded from file
         * this value will be "internal"
         * 
         * @return string path from where the image was loaded
         */
        string getSourceFileLocation();

        /**
         * @brief return std::vector with normalized image data
         * 
         * @return vector<double> std::vector with normalized image data
         */
        vector<double> getDoubleData();

        /**
         * @brief return arma::vector with normalized image data
         * 
         * @return vec arma::vector with normalized image data
         */
        vec getVecDoubleData();

        /**
         * @brief returns an arma::matrix with normalized image data
         * 
         * @return mat arma::matrix with normalized image data with same size as image
         */
        mat getMatDouble();

        /**
         * @brief applies noise to an image
         * 
         * @param noise_value value representing the percentage of the noise to be applied [1,99]
         * @param noise type of noise to be applied
         */
        void applyNoise(double noise_value, noise_type_t noise = noise_type_t::GAUSSIAN);


        /**
         * @brief set the data values of the image from an arma::matrix
         * 
         * @param new_data arma::matrix containing image data
         */
        void setImgDataMatDouble(const mat &new_data);

        /**
         * @brief returns the raw image data
         * 
         * @return const unsigned char* image data
         */
        const unsigned char *getRawImageData();

        /**
         * @brief returns a `string` name for each of the different noise types
         * 
         * @param noise_type type of noise to get the name
         * @return string name of the noise
         */
        static string getNoiseName(noise_type_t noise_type);

        /**
         * @brief saves an image to file as PNG
         * 
         * @param filename full path of the file where the images will be saved
         * @return true if image is saved succesfully
         * @return false if there is a problem saving the image
         */
        bool save(const string filename);
    };
}
#endif
