#ifndef VECIMAGE_H
#define VECIMAGE_H

#define cimg_use_jpeg
#define cimg_display 0 // !! Important: Disables X11 libs so project can build with QT !!

#include <string>
#include <vector>
#include <armadillo>
#include <math.h>
// #include "Noise.hpp"

#include "CImg.h"

namespace AppLogic
{
    using arma::mat;
    using arma::vec;
    using std::string;
    using std::vector;

    // /**
    //  * @brief Internal image object
    //  *
    //  */
    // typedef struct{
    //     std::string source_file_location;
    //     int cols;
    //     int rows;
    //     std::vector<char> data;
    // } VecImage;

    class VecImage
    {
    private:
        std::string source_file_location;
        size_t cols;
        size_t rows;
        cimg_library::CImg<unsigned char> data;

    public:
        // VecImage();
        VecImage(std::string filename);
        // {
        //     try
        //     {
        //         this->data = cimg_library::CImg<unsigned char>(filename.c_str());
        //         this->cols = data.width();
        //         this->rows = data.height();
        //         this->source_file_location = filename;
        //     }
        //     catch (...)
        //     {
        //         this->cols = 0;
        //         this->rows = 0;
        //     }
        // };

        VecImage(vector<double> &img_data, size_t width, size_t height);
        // {
        //     this->data = cimg_library::CImg<unsigned char>(width, height, 1, 1); //construct grayscale image

        //     size_t vec_it = 0;
        //     for (cimg_library::CImg<unsigned char>::iterator pix = data.begin(); pix < data.end(); ++pix)
        //     {
        //         char s = char(img_data[vec_it] * 255);
        //         *pix = s;
        //         ++vec_it;
        //     }
        //     this->source_file_location = "internal";
        //     this->cols = width;
        //     this->rows = height;
        // };

        VecImage(const vec &img_data, size_t width, size_t height);
        // {
        //     this->data = cimg_library::CImg<unsigned char>(width, height, 1, 1); //construct grayscale image

        //     size_t vec_it = 0;
        //     for (cimg_library::CImg<unsigned char>::iterator pix = data.begin(); pix < data.end(); ++pix)
        //     {
        //         char s = char(img_data[vec_it] * 255);
        //         *pix = s;
        //         ++vec_it;
        //     }
        //     this->source_file_location = "internal";
        //     this->cols = width;
        //     this->rows = height;
        // };

        vector<double> get_double_data();
        // {
        //     std::vector<double> result(this->cols * this->rows);
        //     for (size_t y_coord = 0; y_coord < this->rows; y_coord++)
        //     {
        //         for (size_t x_coord = 0; x_coord < this->cols; x_coord++)
        //         {
        //             // unsigne char d = data.at(i, j);
        //             result[y_coord * this->cols + x_coord] = double(data(x_coord, y_coord)) / 255;
        //         }
        //     }
        //     return result;
        // };

        vec getVecDoubleData();
        // {
        //     vec result(this->cols * this->rows);
        //     for (size_t y_coord = 0; y_coord < this->rows; y_coord++)
        //     {
        //         for (size_t x_coord = 0; x_coord < this->cols; x_coord++)
        //         {
        //             // unsigne char d = data.at(i, j);
        //             result[y_coord * this->cols + x_coord] = double(data(x_coord, y_coord)) / 255;
        //         }
        //     }
        //     return result;
        // };

        // void set_double_data(std::vector<double> img_data){
        //     for (size_t i = 0; i < rows; i++)
        //     {
        //         for (size_t j = 0; j < cols; j++)
        //         {
        //             data.at(i ,j) = unsigned char(img_data[i*cols + j] * 255);
        //             // result.push_back(double(data.at(i ,j)/255));
        //         }
        //     }
        // };
        mat getMatDouble();
        void applyNoise();
        void display();
        //     {
        //         data.display(source_file_location.c_str());
        //     }

        //     size_t num_rows() { return rows; };
        //     size_t num_cols() { return cols; };
        // };
        size_t num_rows(); // { return rows; };
        size_t num_cols(); // { return cols; };
        void setImgDataMatDouble(const mat &new_data);
    };
}
#endif
